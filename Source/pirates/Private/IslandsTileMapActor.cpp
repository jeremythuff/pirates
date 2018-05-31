// Fill out your copyright notice in the Description page of Project Settings.

#include "IslandsTileMapActor.h"

#include "PaperTileMapComponent.h"

// Sets default values
AIslandsTileMapActor::AIslandsTileMapActor() : Super()
{
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = false;

  UE_LOG(LogTemp, Log, TEXT("Creating tile map component..."));
  RootComponent = TileMapComponent = CreateDefaultSubobject<UPaperTileMapComponent>(TEXT("TileMapComponent"));
}

void AIslandsTileMapActor::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
}

void AIslandsTileMapActor::PreInitializeComponents()
{
  Super::PreInitializeComponents();
}

void AIslandsTileMapActor::PostInitializeComponents()
{
  Super::PostInitializeComponents();
}

void AIslandsTileMapActor::PostRegisterAllComponents()
{
  Super::PostRegisterAllComponents();
}

TArray<FPaperTileInfo> AIslandsTileMapActor::FindTileInfoAtWorldLoation_Implementation(FVector WorldLocation)
{
  TArray<FPaperTileInfo> TilesAtLocation;

  for (auto LayerItr(TileMapComponent->TileMap->TileLayers.CreateIterator()); LayerItr; LayerItr++)
  {
    int32 LayerIndex = LayerItr.GetIndex();
    if (!(*LayerItr)->IsValidLowLevel())
    {
      continue;
    }

    for (int32 TileX = 0; TileX < TileMapComponent->TileMap->MapHeight; TileX++)
    {
      for (int32 TileY = 0; TileY < TileMapComponent->TileMap->MapWidth; TileY++)
      {
        FVector CenterOfTile = TileMapComponent->GetTileCenterPosition(TileX, TileY, LayerIndex, true);

        if (FVector::DistXY(CenterOfTile, WorldLocation) < TileMapComponent->TileMap->TileWidth / 2)
        {
          FPaperTileInfo TileInfo = TileMapComponent->GetTile(TileX, TileY, LayerIndex);
          if (TileInfo.IsValid())
          {
            TilesAtLocation.Add(FPaperTileInfo(TileInfo));
          }
        }
      }
    }
  }

  return TilesAtLocation;
}

// Called when the game starts or when spawned
void AIslandsTileMapActor::BeginPlay()
{
  Super::BeginPlay();
}
