// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralIslandsTileMapActor.h"

// Sets default values
AProceduralIslandsTileMapActor::AProceduralIslandsTileMapActor() : Super()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GeneratedMap = CreateDefaultSubobject<UPaperTileMap>(TEXT("Generated Tile Map"));
	BaseTileMap->SetTileMap(GeneratedMap);
	
}

// Called every frame
void AProceduralIslandsTileMapActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProceduralIslandsTileMapActor::PostInitProperties()
{
	Super::PostInitProperties();
	Init();
	Generate();
}

#if WITH_EDITOR
void AProceduralIslandsTileMapActor::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent)
{
	Generate();
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

// Initialize Islands Map
void AProceduralIslandsTileMapActor::Init()
{
	#if WITH_EDITOR
	if (!Initialized) {
		Initialized = true;
	#endif
		BaseTileMap->MakeTileMapEditable();
		BaseTileMap->TileMap->TileLayers.Empty();

		UPaperTileLayer *StructuresLayer = BaseTileMap->TileMap->AddNewLayer();
		StructuresLayer->SetLayerCollides(true);
		StructuresLayer->LayerName = FText::FromString("Structures");

		UPaperTileLayer *FoliageLayer = BaseTileMap->TileMap->AddNewLayer();
		FoliageLayer->SetLayerCollides(true);
		FoliageLayer->LayerName = FText::FromString("Foliage");

		UPaperTileLayer *LandLayer = BaseTileMap->TileMap->AddNewLayer();
		LandLayer->SetLayerCollides(true);
		LandLayer->LayerName = FText::FromString("Land");

		UPaperTileLayer *ShallowsLayer = BaseTileMap->TileMap->AddNewLayer();
		FLinearColor TransparentColor = FLinearColor(ShallowsLayer->GetLayerColor());
		TransparentColor.A = 0.9f;
		ShallowsLayer->SetLayerColor(TransparentColor);
		ShallowsLayer->SetLayerCollides(false);
		ShallowsLayer->LayerName = FText::FromString("Shallows");

		UPaperTileLayer *OceanLayer = BaseTileMap->TileMap->AddNewLayer();
		OceanLayer->SetLayerCollides(true);
		OceanLayer->LayerName = FText::FromString("Ocean");

	#if WITH_EDITOR
	}
	#endif
}

// Generate Islands Map
void AProceduralIslandsTileMapActor::Generate()
{
	if (BaseTileSet)
	{

		if (!BaseTileMap->TileMap) {
			BaseTileMap->SetTileMap(GeneratedMap);
		}
			
		std::srand(Seed);
		Resize();

		Matrix noise = TrimNoise(GenerateNoise(Seed), SeaLevel);

		float elevation = 1000.0f;

		Matrix elevatedNoise = CopyNoise(noise);

		// TMap<FString, FString> Edges = InitialEdges();

		for (int32 TileX = 0; TileX < Rows; TileX++)
		{
			for (int32 TileY = 0; TileY < Columns; TileY++)
			{
				float altitude = noise[TileX][TileY];

				AddTile(TileX, TileY, 4, WaterTileIndex);

				if (altitude > SeaLevel)
				{
					FString surroundings = GetSurroundings(noise, TileX, TileY, SeaLevel);
					AddTile(TileX, TileY, 3, FCString::Atoi(*ShallowsEdgeMap.FindRef(Edges.FindRef(surroundings))));

					if (surroundings.Equals(TEXT("00000000")))
					{
						elevatedNoise[TileX][TileY] += elevation;
					}
				}
			}
		}

		float LandLevel = SeaLevel + elevation;

		elevatedNoise = TrimNoise(elevatedNoise, LandLevel);

		for (int32 TileX = 0; TileX < Rows; TileX++)
		{
			for (int32 TileY = 0; TileY < Columns; TileY++)
			{
				float altitude = elevatedNoise[TileX][TileY];

				if (altitude > LandLevel)
				{
					FString surroundings = GetSurroundings(elevatedNoise, TileX, TileY, LandLevel);
					AddTile(TileX, TileY, 2, FCString::Atoi(*LandEdgeMap.FindRef(Edges.FindRef(surroundings))));
				}
			}
		}

		BaseTileMap->RebuildCollision();
	}
}

// Called when the game starts or when spawned
void AProceduralIslandsTileMapActor::BeginPlay()
{
	Super::BeginPlay();
}

void AProceduralIslandsTileMapActor::Resize()
{
	BaseTileMap->ResizeMap(0, 0);
	BaseTileMap->ResizeMap(Rows, Columns);

	BaseTileMap->TileMap->SeparationPerLayer = 5.0f;

	BaseTileMap->TileMap->TileWidth = BaseTileSet->GetTileSize().X;
	BaseTileMap->TileMap->TileHeight = BaseTileSet->GetTileSize().Y;
	
}

Matrix AProceduralIslandsTileMapActor::GenerateNoise(int seed)
{
	Perlin::Reseed(seed);
	int c = 0;
	Matrix noise;
	noise.resize(Rows);
	for (int x = 0; x < Rows; x++)
	{
		noise[x].resize(Columns);
		for (int y = 0; y < Columns; y++)
		{
			float fx = (float)Rows / Frequency;
			float fy = (float)Columns / Frequency;
			float n = Perlin::Noise((float)x / fx, (float)y / fy, Octaves) * 1000.0f;
			noise[x][y] = Density > c++ ? std::abs(n) : n;
			if (c > 100)
			{
				c = 0;
			}
		}
	}
	return noise;
}

Matrix AProceduralIslandsTileMapActor::TrimNoise(Matrix noise, float level)
{
	for (int32 i = 0; i < 12; i++)
	{
		for (int32 TileX = 0; TileX < Rows; TileX++)
		{
			for (int32 TileY = 0; TileY < Columns; TileY++)
			{
				if (TileX > 0 && TileX < Rows - 1 && TileY > 0 && TileY < Columns - 1)
				{
					if (noise[TileX + 1][TileY + 1] < level && noise[TileX - 1][TileY - 1] < level)
					{
						noise[TileX][TileY] = level - 1.0f;
					}
					if (noise[TileX + 1][TileY - 1] < level && noise[TileX - 1][TileY + 1] < level)
					{
						noise[TileX][TileY] = level - 1.0f;
					}
				}
				if (TileX > 0 && TileX < Rows - 1 && noise[TileX + 1][TileY] < level && noise[TileX - 1][TileY] < level)
				{
					noise[TileX][TileY] = level - 1.0f;
				}
				if (TileY > 0 && TileY < Columns - 1 && noise[TileX][TileY + 1] < level && noise[TileX][TileY - 1] < level)
				{
					noise[TileX][TileY] = level - 1.0f;
				}
			}
		}
	}
	return noise;
}

FString AProceduralIslandsTileMapActor::GetSurroundings(Matrix noise, int32 x, int32 y, float level)
{
	int top = (x == 0) ? x : x - 1;
	int bottom = (x == Rows - 1) ? x : x + 1;
	int left = (y == 0) ? y : y - 1;
	int right = (y == Columns - 1) ? y : y + 1;
	FString surroundings = TEXT("");
	surroundings += noise[top][left] <= level ? TEXT("1") : TEXT("0");
	surroundings += noise[top][y] <= level ? TEXT("1") : TEXT("0");
	surroundings += noise[top][right] <= level ? TEXT("1") : TEXT("0");
	surroundings += noise[x][right] <= level ? TEXT("1") : TEXT("0");
	surroundings += noise[bottom][right] <= level ? TEXT("1") : TEXT("0");
	surroundings += noise[bottom][y] <= level ? TEXT("1") : TEXT("0");
	surroundings += noise[bottom][left] <= level ? TEXT("1") : TEXT("0");
	surroundings += noise[x][left] <= level ? TEXT("1") : TEXT("0");
	return surroundings;
}

Matrix AProceduralIslandsTileMapActor::CopyNoise(Matrix noise)
{
	Matrix copy;
	copy.resize(noise.size());
	for (int x = 0; x < noise.size(); x++)
	{
		copy[x].resize(noise[x].size());
		for (int y = 0; y < noise[x].size(); y++)
		{
			copy[x][y] = noise[x][y];
		}
	}
	return copy;
}

void AProceduralIslandsTileMapActor::AddTile(int32 x, int32 y, int32 layerIndex, int32 tileIndex)
{
	FPaperTileInfo tile;
	tile.TileSet = BaseTileSet;
	tile.PackedTileIndex = tileIndex;
	BaseTileMap->SetTile(x, y, layerIndex, tile);
}
