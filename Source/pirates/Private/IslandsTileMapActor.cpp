// Fill out your copyright notice in the Description page of Project Settings.

#include "IslandsTileMapActor.h"

#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "PaperTileMap.h"
#include "PaperTileSet.h"
#include "PaperTileLayer.h"
#include "PaperTileMapComponent.h"

// Sets default values
AIslandsTileMapActor::AIslandsTileMapActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = BaseTileMap = CreateDefaultSubobject<UPaperTileMapComponent>(TEXT("BaseTileMap"));
}

// Called every frame
void AIslandsTileMapActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AIslandsTileMapActor::PostInitProperties()
{
	Super::PostInitProperties();
}

#if WITH_EDITOR
void AIslandsTileMapActor::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif


TArray<FPaperTileInfo> AIslandsTileMapActor::FindTileInfoAtWorldLoation_Implementation(FVector WorldLocation)
{
	TArray<FPaperTileInfo> TilesAtLocation;

	UPaperTileMap* TileMap = BaseTileMap->TileMap;
	int32 MapHeight = TileMap->MapHeight;
	int32 MapWidth = TileMap->MapWidth;

	TArray<UPaperTileLayer*> Layers = TileMap->TileLayers;

	for (auto LayerItr(Layers.CreateIterator()); LayerItr; LayerItr++)
	{
		int32 LayerIndex = LayerItr.GetIndex();
		if (!(*LayerItr)->IsValidLowLevel()) continue;

		for (int32 TileX = 0; TileX < MapHeight; TileX++) {

			for (int32 TileY = 0; TileY < MapWidth; TileY++) {

				FVector CenterOfTile = BaseTileMap->GetTileCenterPosition(TileX, TileY, LayerIndex, true);

				if (FVector::DistXY(CenterOfTile, WorldLocation) < TileMap->TileWidth / 2) {
					FPaperTileInfo TileInfo = BaseTileMap->GetTile(TileX, TileY, LayerIndex);
					if (TileInfo.IsValid()) {
						TilesAtLocation.Insert(TileInfo, LayerIndex);
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
