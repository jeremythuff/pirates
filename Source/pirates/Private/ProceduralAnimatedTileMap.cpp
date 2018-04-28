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

	AProceduralAnimatedTileMap::GenerateMap();

	AProceduralAnimatedTileMap::SetUpMapAnimation();

}

void AProceduralAnimatedTileMap::GenerateMap() {

	FastNoise = NewObject<UFastNoise>(RootComponent, TEXT("NoiseGenerator"));

	BaseTileMap->MakeTileMapEditable();

	BaseTileMap->ResizeMap(Rows, Columns);

	UPaperTileLayer* OceanLayer = BaseTileMap->TileMap->AddNewLayer();
	UPaperTileLayer* GroundLayer = BaseTileMap->TileMap->AddNewLayer();
	GroundLayer->SetLayerCollides(true);

	if (BaseTileSet) {

		BaseTileMap->MakeTileMapEditable();
		BaseTileMap->TileMap->SelectedTileSet = BaseTileSet;

		TMultiMap<FString, int32> TileTypes;

		for (int32 index = 0; index < BaseTileSet->GetTileCount(); index++) {

			FName TileUserMetadata = BaseTileSet->GetTileUserData(index);
			FString TileUserMetadaRaw = TileUserMetadata.ToString();
			
			if (!TileUserMetadaRaw.Contains("tiletype")) {
				continue;
			} 

			TArray<FString> Metadatum;
			TileUserMetadaRaw.ParseIntoArray(Metadatum, TEXT(";"), true);

			for (auto MetadatumIter(Metadatum.CreateIterator()); MetadatumIter; MetadatumIter++) {
				if (!MetadatumIter->Contains("tiletype=")) {
					continue;
				}

				FString Key, TileType;
				MetadatumIter->Split(TEXT("="), &Key, &TileType);
				
				TileTypes.AddUnique(TileType,index);

			}

		}


		
		int32 TileSize = BaseTileSet->GetTileSize().GetMax();

		BaseTileMap->TileMap->TileWidth = TileSize;
		BaseTileMap->TileMap->TileHeight = TileSize;

		FastNoise->SetSeed(FMath::RandRange(0,10));
		FastNoise->SetFrequency(float(1 - (1 / Columns*Rows)));
		FastNoise->SetFractalOctaves(1 - (1 / Columns*Rows)*10);
		FastNoise->SetFractalGain(0.5f);
		FastNoise->SetFractalLacunarity(2.0f);
			
		/*FastNoise->SetCellularNoiseLookup(FastNoise);*/
		/*FastNoise->SetCellularDistanceFunction(ECellularDistanceFunction::Natural);
		FastNoise->SetCellularReturnType(ECellularReturnType::Distance2Add);*/

		FastNoise->SetNoiseType(ENoiseType::Value);

		for (int32 TileX = 0; TileX < Rows; TileX++) {
			for (int32 TileY = 0; TileY < Columns; TileY++) {

				float noise = (FastNoise->GetNoise((float)TileX, (float)TileY));

				FPaperTileInfo WaterTileInfo = FPaperTileInfo();
				WaterTileInfo.TileSet = BaseTileSet;
				WaterTileInfo.PackedTileIndex = 72;
				
				UE_LOG(LogTemp, Warning, TEXT("Tile %dX %dY Layer %d, at %d pixels and %f noise"), TileX, TileY, OceanLayer->GetLayerIndex(), TileSize, noise);
				
				BaseTileMap->SetTile(TileX, TileY, OceanLayer->GetLayerIndex(), WaterTileInfo);
				
				if (noise>0.75f) {
					FPaperTileInfo GroundTileInfo = FPaperTileInfo();
					GroundTileInfo.TileSet = BaseTileSet;
					TArray<int32> GrassTileArray;
					TileTypes.MultiFind("grass", GrassTileArray, true);
					int32 GrassTileIndex = FMath::RandRange(0, GrassTileArray.Num() - 1);
					GroundTileInfo.PackedTileIndex = GrassTileArray[GrassTileIndex];
					BaseTileMap->SetTile(TileX, TileY, GroundLayer->GetLayerIndex(), GroundTileInfo);
				}

			}
		}

		

	}

	BaseTileMap->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BaseTileMap->SetDefaultCollisionThickness(1000.0f);
	BaseTileMap->RebuildCollision();

}