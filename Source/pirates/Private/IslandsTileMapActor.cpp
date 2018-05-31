// Fill out your copyright notice in the Description page of Project Settings.

#include "IslandsTileMapActor.h"

#include "PaperTileMapComponent.h"

// Sets default values
AIslandsTileMapActor::AIslandsTileMapActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	UE_LOG(LogTemp, Log, TEXT("Creating tile map component..."));
	TileMapComponent = CreateDefaultSubobject<UPaperTileMapComponent>(TEXT("TileMapComponent"));
}

// Called every frame
void AIslandsTileMapActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AIslandsTileMapActor::PostRegisterAllComponents()
{
	Super::PostRegisterAllComponents();
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

	UPaperTileMap* TileMap = TileMapComponent->TileMap;
	int32 MapHeight = TileMap->MapHeight;
	int32 MapWidth = TileMap->MapWidth;

	TArray<UPaperTileLayer*> Layers = TileMap->TileLayers;

	for (auto LayerItr(Layers.CreateIterator()); LayerItr; LayerItr++)
	{
		int32 LayerIndex = LayerItr.GetIndex();
		if (!(*LayerItr)->IsValidLowLevel()) continue;

		for (int32 TileX = 0; TileX < MapHeight; TileX++) {

			for (int32 TileY = 0; TileY < MapWidth; TileY++) {

				FVector CenterOfTile = TileMapComponent->GetTileCenterPosition(TileX, TileY, LayerIndex, true);

				if (FVector::DistXY(CenterOfTile, WorldLocation) < TileMap->TileWidth / 2) {
					FPaperTileInfo TileInfo = TileMapComponent->GetTile(TileX, TileY, LayerIndex);
					if (TileInfo.IsValid()) {
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
