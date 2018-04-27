// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralAnimatedTileMap.h"
#include "PaperTileMap.h"
#include "PaperTileMapComponent.h"
#include "PaperTileSet.h"
#include "Fastnoise/FastNoise.h"




AProceduralAnimatedTileMap::AProceduralAnimatedTileMap() :
	Super()
{

	if (BaseTileMap != NULL) {
		BaseTileMap->DestroyComponent();
	}
	
	BaseTileMap = CreateDefaultSubobject<UPaperTileMapComponent>(TEXT("Generated Map Component"));
	BaseTileMap->SetupAttachment(RootComponent);

	UPaperTileMap* GeneratedMap = CreateDefaultSubobject<UPaperTileMap>(TEXT("Generated Tile Map"));
	BaseTileMap->SetTileMap(GeneratedMap);
		
}

// Called when the game starts or when spawned
void AProceduralAnimatedTileMap::BeginPlay()
{
	Super::BeginPlay();

	AProceduralAnimatedTileMap::GenerateMap();

	AProceduralAnimatedTileMap::SetUpMapAnimation();

}

void AProceduralAnimatedTileMap::GenerateMap() {

	FastNoise = NewObject<UFastNoise>(RootComponent, TEXT("NoiseGenerator"));

	BaseTileMap->MakeTileMapEditable();

	BaseTileMap->ResizeMap(Rows, Columns);

	UPaperTileLayer* OceanLayer = BaseTileMap->TileMap->AddNewLayer();

	if (BaseTileSet) {

		BaseTileMap->MakeTileMapEditable();
		BaseTileMap->TileMap->SelectedTileSet = BaseTileSet;
		
		int32 TileSize = BaseTileSet->GetTileSize().GetMax();

		BaseTileMap->TileMap->TileWidth = TileSize;
		BaseTileMap->TileMap->TileHeight = TileSize;
			
		for (int32 TileX = 0; TileX < Rows; TileX++) {
			for (int32 TileY = 0; TileY < Columns; TileY++) {
	
				float noise = FastNoise->GetNoise((float)TileX, (float)TileY);

				FPaperTileInfo TileInfo = FPaperTileInfo();
				TileInfo.TileSet = BaseTileSet;
				TileInfo.PackedTileIndex = 72;
				UE_LOG(LogTemp, Warning, TEXT("Tile %dX %dY Layer %d, at %d pixels and %f noise"), TileX, TileY, OceanLayer->GetLayerIndex(), TileSize, noise);
				BaseTileMap->SetTile(TileX, TileY, OceanLayer->GetLayerIndex(), TileInfo);
			}
		}
	}

}