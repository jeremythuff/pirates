// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralIslandsTileMapActor.h"
#include "ConstructorHelpers.h"

DEFINE_LOG_CATEGORY(ProceduralLog);

// Sets default values
AProceduralIslandsTileMapActor::AProceduralIslandsTileMapActor() : Super()
{
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = false;

  static ConstructorHelpers::FObjectFinder<UPaperTileSet> IslandsTileSet(TEXT("/Game/tile_sets/Terrain"));
  if (IslandsTileSet.Succeeded())
  {
    TileSet = IslandsTileSet.Object;
  }

  static ConstructorHelpers::FObjectFinder<UMaterialInterface> TranslucentUnlitSpriteMaterial(TEXT("/Paper2D/TranslucentUnlitSpriteMaterial"));
  if (TranslucentUnlitSpriteMaterial.Succeeded())
  {
    TileMapComponent->SetMaterial(0, TranslucentUnlitSpriteMaterial.Object);
  }
}

void AProceduralIslandsTileMapActor::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
}

void AProceduralIslandsTileMapActor::PreInitializeComponents()
{
  Super::PreInitializeComponents();
}

void AProceduralIslandsTileMapActor::PostInitializeComponents()
{
  Super::PostInitializeComponents();
}

void AProceduralIslandsTileMapActor::PostRegisterAllComponents()
{
  Generate();
  Super::PostRegisterAllComponents();
}

void AProceduralIslandsTileMapActor::Init()
{
  UE_LOG(ProceduralLog, Log, TEXT("Creating tile map..."));
  TileMapComponent->CreateNewTileMap(Rows, Columns, TileSet->GetTileSize().X, TileSet->GetTileSize().Y, 1.0f, false);

  UE_LOG(ProceduralLog, Log, TEXT("Making tile map editable..."));
  TileMapComponent->MakeTileMapEditable();

  TileMapComponent->TileMap->SeparationPerLayer = 5.0f;

  UE_LOG(ProceduralLog, Log, TEXT("Adding layers..."));

  UE_LOG(ProceduralLog, Log, TEXT("Adding structures layer..."));
  UPaperTileLayer *StructuresLayer = TileMapComponent->TileMap->AddNewLayer();
  StructuresLayer->SetLayerCollides(true);
  StructuresLayer->LayerName = FText::FromString("Structures");

  UE_LOG(ProceduralLog, Log, TEXT("Adding foliage layer..."));
  UPaperTileLayer *FoliageLayer = TileMapComponent->TileMap->AddNewLayer();
  FoliageLayer->SetLayerCollides(true);
  FoliageLayer->LayerName = FText::FromString("Foliage");

  UE_LOG(ProceduralLog, Log, TEXT("Adding land layer..."));
  UPaperTileLayer *LandLayer = TileMapComponent->TileMap->AddNewLayer();
  LandLayer->SetLayerCollides(true);
  LandLayer->LayerName = FText::FromString("Land");

  UE_LOG(ProceduralLog, Log, TEXT("Adding shallows layer..."));
  UPaperTileLayer *ShallowsLayer = TileMapComponent->TileMap->AddNewLayer();
  FLinearColor TransparentColor = FLinearColor(ShallowsLayer->GetLayerColor());
  TransparentColor.A = 0.9f;
  ShallowsLayer->SetLayerColor(TransparentColor);
  ShallowsLayer->SetLayerCollides(false);
  ShallowsLayer->LayerName = FText::FromString("Shallows");

  UE_LOG(ProceduralLog, Log, TEXT("Adding ocean layer..."));
  UPaperTileLayer *OceanLayer = TileMapComponent->TileMap->AddNewLayer();
  OceanLayer->SetLayerCollides(true);
  OceanLayer->LayerName = FText::FromString("Ocean");
}

// Generate Islands Map
void AProceduralIslandsTileMapActor::Generate()
{
  if (TileSet)
  { 
    Init();
    Resize();
    Reposition();

    UE_LOG(ProceduralLog, Log, TEXT("Generating tiles..."));
    Matrix Noise = TrimNoise(GenerateNoise(), SeaLevel);

    Matrix ElevatedNoise = CopyNoise(Noise);

    // TMap<FString, FString> Edges = InitialEdges();

    for (int32 x = 0; x < Rows; x++)
    {
      for (int32 y = 0; y < Columns; y++)
      {
        AddTile(x, y, 4, WaterTileIndex);

        if (Noise[x][y] > SeaLevel)
        {
          FString Surroundings = GetSurroundings(Noise, x, y, SeaLevel);
          AddTile(x, y, 3, FCString::Atoi(*ShallowsEdgeMap.FindRef(Edges.FindRef(Surroundings))));

          if (Surroundings.Equals(TEXT("00000000")))
          {
            ElevatedNoise[x][y] += Scale;

            if (std::rand() % 100 <= ShallowsRockLikelihood * 100)
            {
              AddTile(x, y, 2, ShallowsRockTileIndeces[std::rand() % ShallowsRockTileIndeces.Num()]);
            }
          }
        }
      }
    }

    float LandLevel = SeaLevel + Scale;

    ElevatedNoise = TrimNoise(ElevatedNoise, LandLevel);

    for (int32 x = 0; x < Rows; x++)
    {
      for (int32 y = 0; y < Columns; y++)
      {
        if (ElevatedNoise[x][y] > LandLevel)
        {
          FString Surroundings = GetSurroundings(ElevatedNoise, x, y, LandLevel);

          int32 TileIndex;

          if (Surroundings.Equals(TEXT("00000000")))
          {
            ElevatedNoise[x][y] += Scale;
            TileIndex = GrassTileIndeces[std::rand() % GrassTileIndeces.Num()];
          }
          else
          {
            TileIndex = FCString::Atoi(*LandEdgeMap.FindRef(Edges.FindRef(Surroundings)));
          }

          AddTile(x, y, 2, TileIndex);
        }
      }
    }

    float FoliageLevel = LandLevel + Scale;

    ElevatedNoise = TrimNoise(ElevatedNoise, FoliageLevel);

    for (int32 x = 0; x < Rows; x++)
    {
      for (int32 y = 0; y < Columns; y++)
      {
        if (ElevatedNoise[x][y] > FoliageLevel)
        {
          FString Surroundings = GetSurroundings(ElevatedNoise, x, y, FoliageLevel);

          if (Surroundings.Equals(TEXT("00000000")))
          {
            AddTile(x, y, 1, FoilageTileIndeces[std::rand() % FoilageTileIndeces.Num()]);
          }
          else
          {
            AddTile(x, y, 2, GrassTileIndeces[std::rand() % GrassTileIndeces.Num()]);
          }
        }
      }
    }

    UE_LOG(ProceduralLog, Log, TEXT("Rebuilding collision..."));
    TileMapComponent->RebuildCollision();
  }
  else
  {
    UE_LOG(ProceduralLog, Log, TEXT("No tile set..."));
  }
}

// Called when the game starts or when spawned
void AProceduralIslandsTileMapActor::BeginPlay()
{
  Super::BeginPlay();
}

void AProceduralIslandsTileMapActor::Resize()
{
  UE_LOG(ProceduralLog, Log, TEXT("Resizing map..."));
  TileMapComponent->ResizeMap(0, 0);
  TileMapComponent->ResizeMap(Rows, Columns);
}

void AProceduralIslandsTileMapActor::Reposition()
{
  UE_LOG(ProceduralLog, Log, TEXT("Repositioning map..."));
  float MapWidth = (float)Columns * (float)TileSet->GetTileSize().X;
  float MapHeight = (float)Rows * (float)TileSet->GetTileSize().Y;

  TileMapComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, -90.0f));
  TileMapComponent->SetRelativeLocation(FVector(MapWidth, (MapWidth / 2.0f) * (-1.0f), 0.0f));
}

Matrix AProceduralIslandsTileMapActor::GenerateNoise()
{
  std::srand(Seed);
  std::vector<FVector2D> octavesOffsets;
  octavesOffsets.resize(Octaves);
  for (int i = 0; i < Octaves; i++)
  {
    octavesOffsets[i].X = (std::rand() % (200001) - 100000) + Offset.X;
    octavesOffsets[i].Y = (std::rand() % (200001) - 100000) + Offset.Y;
  }

  float maxNoiseHeight = std::numeric_limits<float>::min();
  float minNoiseHeight = std::numeric_limits<float>::max();

  float halfWidth = (float) Columns / 2.0f;
  float halfHeight = (float) Rows / 2.0f;

  if (Scale <= 0) {
    Scale = 0.0001f;
  }

  Matrix Noise;
  Noise.resize(Rows);
  for (int x = 0; x < Rows; x++)
  {
    Noise[x].resize(Columns);
    for (int y = 0; y < Columns; y++)
    {
      float amplitude = 1.0f;
      float frequency = 1.0f;
      float noiseHeight = 0.0f;

      for (int i = 0; i < Octaves; i++)
      {
        float sampleX = (x - halfWidth) / Scale * frequency + octavesOffsets[i].X;
        float sampleY = (y - halfHeight) / Scale * frequency + octavesOffsets[i].Y;

        float perlinValue = Perlin::Noise(sampleX, sampleY) * 2 - 1;
        noiseHeight += perlinValue * amplitude;

        amplitude *= Persistance;
        frequency *= Lacunarity;
      }

      if (noiseHeight > maxNoiseHeight)
      {
        maxNoiseHeight = noiseHeight;
      }
      else if (noiseHeight < minNoiseHeight)
      {
        minNoiseHeight = noiseHeight;
      }

      Noise[x][y] = noiseHeight;
    }
  }
  for (int x = 0; x < Rows; x++)
  {
    for (int y = 0; y < Columns; y++)
    {
      Noise[x][y] = Perlin::InverseLerp(minNoiseHeight, maxNoiseHeight, Noise[x][y]);
    }
  }
  return Noise;
}

Matrix AProceduralIslandsTileMapActor::TrimNoise(Matrix Noise, float Level)
{
  for (int32 i = 0; i < 12; i++)
  {
    for (int32 x = 0; x < Rows; x++)
    {
      for (int32 y = 0; y < Columns; y++)
      {
        if (x > 0 && x < Rows - 1 && y > 0 && y < Columns - 1)
        {
          if (Noise[x + 1][y + 1] < Level && Noise[x - 1][y - 1] < Level)
          {
            Noise[x][y] = Level - 1.0f;
          }
          if (Noise[x + 1][y - 1] < Level && Noise[x - 1][y + 1] < Level)
          {
            Noise[x][y] = Level - 1.0f;
          }
        }
        if (x > 0 && x < Rows - 1 && Noise[x + 1][y] < Level && Noise[x - 1][y] < Level)
        {
          Noise[x][y] = Level - 1.0f;
        }
        if (y > 0 && y < Columns - 1 && Noise[x][y + 1] < Level && Noise[x][y - 1] < Level)
        {
          Noise[x][y] = Level - 1.0f;
        }
      }
    }
  }
  return Noise;
}

FString AProceduralIslandsTileMapActor::GetSurroundings(Matrix Noise, int32 x, int32 y, float Level)
{
  int Top = (x == 0) ? x : x - 1;
  int Bottom = (x == Rows - 1) ? x : x + 1;
  int Left = (y == 0) ? y : y - 1;
  int Right = (y == Columns - 1) ? y : y + 1;
  FString Surroundings = TEXT("");
  Surroundings += Noise[Top][Left] <= Level ? TEXT("1") : TEXT("0");
  Surroundings += Noise[Top][y] <= Level ? TEXT("1") : TEXT("0");
  Surroundings += Noise[Top][Right] <= Level ? TEXT("1") : TEXT("0");
  Surroundings += Noise[x][Right] <= Level ? TEXT("1") : TEXT("0");
  Surroundings += Noise[Bottom][Right] <= Level ? TEXT("1") : TEXT("0");
  Surroundings += Noise[Bottom][y] <= Level ? TEXT("1") : TEXT("0");
  Surroundings += Noise[Bottom][Left] <= Level ? TEXT("1") : TEXT("0");
  Surroundings += Noise[x][Left] <= Level ? TEXT("1") : TEXT("0");
  return Surroundings;
}

Matrix AProceduralIslandsTileMapActor::CopyNoise(Matrix Noise)
{
  Matrix Copy;
  Copy.resize(Noise.size());
  for (int x = 0; x < Noise.size(); x++)
  {
    Copy[x].resize(Noise[x].size());
    for (int y = 0; y < Noise[x].size(); y++)
    {
      Copy[x][y] = Noise[x][y];
    }
  }
  return Copy;
}

void AProceduralIslandsTileMapActor::AddTile(int32 x, int32 y, int32 LayerIndex, int32 TileIndex)
{
  FPaperTileInfo Tile;
  Tile.TileSet = TileSet;
  Tile.PackedTileIndex = TileIndex;
  TileMapComponent->SetTile(x, y, LayerIndex, Tile);
}