// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralIslandsTileMapActor.h"
#include "ConstructorHelpers.h"

DEFINE_LOG_CATEGORY(ProceduralLog);

// Sets default values
AProceduralIslandsTileMapActor::AProceduralIslandsTileMapActor() : Super()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	UE_LOG(ProceduralLog, Log, TEXT("Creating tile map..."));
	UPaperTileMap *GeneratedMap = CreateDefaultSubobject<UPaperTileMap>(TEXT("GeneratedMap"));

	UE_LOG(ProceduralLog, Log, TEXT("Setting tile map..."));
	GetTileMapComponent()->SetTileMap(GeneratedMap);

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> TranslucentUnlitSpriteMaterial(TEXT("/Paper2D/TranslucentUnlitSpriteMaterial"));
	if (TranslucentUnlitSpriteMaterial.Succeeded())
	{
		GetTileMapComponent()->SetMaterial(0, TranslucentUnlitSpriteMaterial.Object);
	}

	static ConstructorHelpers::FObjectFinder<UPaperTileSet> IslandsTileSet(TEXT("/Game/tile_sets/Terrain"));
	if (IslandsTileSet.Succeeded())
	{
		TileSet = IslandsTileSet.Object;
	}
}

// Called every frame
void AProceduralIslandsTileMapActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProceduralIslandsTileMapActor::PostRegisterAllComponents()
{
	Init();
	Generate();
	Super::PostRegisterAllComponents();
}

#if WITH_EDITOR
void AProceduralIslandsTileMapActor::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	Generate();
}
#endif

void AProceduralIslandsTileMapActor::Init()
{
	UE_LOG(ProceduralLog, Log, TEXT("Layer count: %d"), GetTileMapComponent()->TileMap->TileLayers.Num());

	if (GetTileMapComponent()->TileMap->TileLayers.Num() != 5)
	{

		UE_LOG(ProceduralLog, Log, TEXT("Making tile map editable..."));

		GetTileMapComponent()->MakeTileMapEditable();

		GetTileMapComponent()->TileMap->TileLayers.Empty();

		UE_LOG(ProceduralLog, Log, TEXT("Adding layers..."));

		UE_LOG(ProceduralLog, Log, TEXT("Adding structures layer..."));
		UPaperTileLayer *StructuresLayer = GetTileMapComponent()->TileMap->AddNewLayer();
		StructuresLayer->SetLayerCollides(true);
		StructuresLayer->LayerName = FText::FromString("Structures");

		UE_LOG(ProceduralLog, Log, TEXT("Adding foliage layer..."));
		UPaperTileLayer *FoliageLayer = GetTileMapComponent()->TileMap->AddNewLayer();
		FoliageLayer->SetLayerCollides(true);
		FoliageLayer->LayerName = FText::FromString("Foliage");

		UE_LOG(ProceduralLog, Log, TEXT("Adding land layer..."));
		UPaperTileLayer *LandLayer = GetTileMapComponent()->TileMap->AddNewLayer();
		LandLayer->SetLayerCollides(true);
		LandLayer->LayerName = FText::FromString("Land");

		UE_LOG(ProceduralLog, Log, TEXT("Adding shallows layer..."));
		UPaperTileLayer *ShallowsLayer = GetTileMapComponent()->TileMap->AddNewLayer();
		FLinearColor TransparentColor = FLinearColor(ShallowsLayer->GetLayerColor());
		TransparentColor.A = 0.9f;
		ShallowsLayer->SetLayerColor(TransparentColor);
		ShallowsLayer->SetLayerCollides(false);
		ShallowsLayer->LayerName = FText::FromString("Shallows");

		UE_LOG(ProceduralLog, Log, TEXT("Adding ocean layer..."));
		UPaperTileLayer *OceanLayer = GetTileMapComponent()->TileMap->AddNewLayer();
		OceanLayer->SetLayerCollides(false);
		OceanLayer->LayerName = FText::FromString("Ocean");
	}
}

// Generate Islands Map
void AProceduralIslandsTileMapActor::Generate()
{
	if (TileSet)
	{
		UE_LOG(ProceduralLog, Log, TEXT("Generating map..."));
		std::srand(Seed);
		Resize();
		Reposition();

		Matrix Noise = TrimNoise(GenerateNoise(), SeaLevel);

		Matrix ElevatedNoise = CopyNoise(Noise);

		// TMap<FString, FString> Edges = InitialEdges();

		for (int32 X = 0; X < Rows; X++)
		{
			for (int32 Y = 0; Y < Columns; Y++)
			{
				AddTile(X, Y, 4, WaterTileIndex);

				if (Noise[X][Y] > SeaLevel)
				{
					FString Surroundings = GetSurroundings(Noise, X, Y, SeaLevel);
					AddTile(X, Y, 3, FCString::Atoi(*ShallowsEdgeMap.FindRef(Edges.FindRef(Surroundings))));

					if (Surroundings.Equals(TEXT("00000000")))
					{
						ElevatedNoise[X][Y] += Scale;
					}
				}
			}
		}

		float LandLevel = SeaLevel + Scale;

		ElevatedNoise = TrimNoise(ElevatedNoise, LandLevel);

		for (int32 X = 0; X < Rows; X++)
		{
			for (int32 Y = 0; Y < Columns; Y++)
			{
				if (ElevatedNoise[X][Y] > LandLevel)
				{
					FString Surroundings = GetSurroundings(ElevatedNoise, X, Y, LandLevel);
					AddTile(X, Y, 2, FCString::Atoi(*LandEdgeMap.FindRef(Edges.FindRef(Surroundings))));
				}
			}
		}

		UE_LOG(ProceduralLog, Log, TEXT("Rebuilding collision..."));
		GetTileMapComponent()->RebuildCollision();
	}
	else
	{
		UE_LOG(ProceduralLog, Log, TEXT("Tile set not set..."));
	}
}

// Called when the game starts or when spawned
void AProceduralIslandsTileMapActor::BeginPlay()
{
	Super::BeginPlay();
}

void AProceduralIslandsTileMapActor::Resize()
{

	GetTileMapComponent()->ResizeMap(0, 0);
	GetTileMapComponent()->ResizeMap(Rows, Columns);

	GetTileMapComponent()->TileMap->SeparationPerLayer = 5.0f;

	GetTileMapComponent()->TileMap->TileWidth = TileSet->GetTileSize().X;
	GetTileMapComponent()->TileMap->TileHeight = TileSet->GetTileSize().Y;
}

void AProceduralIslandsTileMapActor::Reposition()
{
	float MapWidth = (float)Columns * (float)TileSet->GetTileSize().X;
	float MapHeight = (float)Rows * (float)TileSet->GetTileSize().Y;

	GetTileMapComponent()->SetRelativeRotation(FRotator(0.0f, 90.0f, -90.0f));
	GetTileMapComponent()->SetRelativeLocation(FVector(MapWidth, (MapWidth / 2.0f) * (-1.0f), 0.0f));
}

Matrix AProceduralIslandsTileMapActor::GenerateNoise()
{
	Perlin::Reseed(Seed);
	int C = 0;
	Matrix Noise;
	Noise.resize(Rows);
	for (int X = 0; X < Rows; X++)
	{
		Noise[X].resize(Columns);
		for (int Y = 0; Y < Columns; Y++)
		{
			float fx = (float)Rows / Frequency;
			float fy = (float)Columns / Frequency;
			float n = Perlin::Noise((float)X / fx, (float)Y / fy, Octaves) * Scale;
			Noise[X][Y] = Density > C++ ? std::abs(n) : n;
			if (C > 100)
			{
				C = 0;
			}
		}
	}
	return Noise;
}

Matrix AProceduralIslandsTileMapActor::TrimNoise(Matrix Noise, float Level)
{
	for (int32 i = 0; i < 12; i++)
	{
		for (int32 X = 0; X < Rows; X++)
		{
			for (int32 Y = 0; Y < Columns; Y++)
			{
				if (X > 0 && X < Rows - 1 && Y > 0 && Y < Columns - 1)
				{
					if (Noise[X + 1][Y + 1] < Level && Noise[X - 1][Y - 1] < Level)
					{
						Noise[X][Y] = Level - 1.0f;
					}
					if (Noise[X + 1][Y - 1] < Level && Noise[X - 1][Y + 1] < Level)
					{
						Noise[X][Y] = Level - 1.0f;
					}
				}
				if (X > 0 && X < Rows - 1 && Noise[X + 1][Y] < Level && Noise[X - 1][Y] < Level)
				{
					Noise[X][Y] = Level - 1.0f;
				}
				if (Y > 0 && Y < Columns - 1 && Noise[X][Y + 1] < Level && Noise[X][Y - 1] < Level)
				{
					Noise[X][Y] = Level - 1.0f;
				}
			}
		}
	}
	return Noise;
}

FString AProceduralIslandsTileMapActor::GetSurroundings(Matrix Noise, int32 X, int32 Y, float Level)
{
	int Top = (X == 0) ? X : X - 1;
	int Bottom = (X == Rows - 1) ? X : X + 1;
	int Left = (Y == 0) ? Y : Y - 1;
	int Right = (Y == Columns - 1) ? Y : Y + 1;
	FString Surroundings = TEXT("");
	Surroundings += Noise[Top][Left] <= Level ? TEXT("1") : TEXT("0");
	Surroundings += Noise[Top][Y] <= Level ? TEXT("1") : TEXT("0");
	Surroundings += Noise[Top][Right] <= Level ? TEXT("1") : TEXT("0");
	Surroundings += Noise[X][Right] <= Level ? TEXT("1") : TEXT("0");
	Surroundings += Noise[Bottom][Right] <= Level ? TEXT("1") : TEXT("0");
	Surroundings += Noise[Bottom][Y] <= Level ? TEXT("1") : TEXT("0");
	Surroundings += Noise[Bottom][Left] <= Level ? TEXT("1") : TEXT("0");
	Surroundings += Noise[X][Left] <= Level ? TEXT("1") : TEXT("0");
	return Surroundings;
}

Matrix AProceduralIslandsTileMapActor::CopyNoise(Matrix Noise)
{
	Matrix Copy;
	Copy.resize(Noise.size());
	for (int X = 0; X < Noise.size(); X++)
	{
		Copy[X].resize(Noise[X].size());
		for (int Y = 0; Y < Noise[X].size(); Y++)
		{
			Copy[X][Y] = Noise[X][Y];
		}
	}
	return Copy;
}

void AProceduralIslandsTileMapActor::AddTile(int32 X, int32 Y, int32 LayerIndex, int32 TileIndex)
{
	FPaperTileInfo Tile;
	Tile.TileSet = TileSet;
	Tile.PackedTileIndex = TileIndex;
	GetTileMapComponent()->SetTile(X, Y, LayerIndex, Tile);
}