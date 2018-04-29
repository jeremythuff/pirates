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
	
	if (BaseTileSet) {

		float MapWidth = (float)Columns*(float)BaseTileSet->GetTileSize().X;
		float MapHeight = (float)Rows*(float)BaseTileSet->GetTileSize().Y;


		/*UE_LOG(LogTemp, Warning, TEXT("MapWidth(%f)"), MapWidth)
		UE_LOG(LogTemp, Warning, TEXT("MapHeight(%f)"), MapHeight)*/

		BaseTileMap->SetRelativeRotation(FRotator(0.0f, 90.0f, -90.0f));
		BaseTileMap->SetRelativeLocation(FVector(MapWidth, (MapWidth/2.0f)*(-1.0f), 0.0f));

		FastNoise = NewObject<UFastNoise>(RootComponent, TEXT("NoiseGenerator"));

		BaseTileMap->MakeTileMapEditable();

		BaseTileMap->ResizeMap(Rows, Columns);

		UPaperTileLayer* OceanLayer = BaseTileMap->TileMap->AddNewLayer(0);
		OceanLayer->SetLayerCollides(false);

		UPaperTileLayer* ShallowsLayer = BaseTileMap->TileMap->AddNewLayer(1);
		ShallowsLayer->SetLayerCollides(false);

		UPaperTileLayer* GroundLayer = BaseTileMap->TileMap->AddNewLayer(2);
		GroundLayer->SetLayerCollides(true);

		BaseTileMap->MakeTileMapEditable();
		BaseTileMap->TileMap->SelectedTileSet = BaseTileSet;

		TileTypes = ExtractAllTileUserData(TEXT("tiletype"));
		
		int32 TileSize = BaseTileSet->GetTileSize().GetMax();

		BaseTileMap->TileMap->TileWidth = TileSize;
		BaseTileMap->TileMap->TileHeight = TileSize;


		//Seed Map with islands.

		FastNoise->SetSeed(Seed);
		FastNoise->SetFrequency(Frequency);
		FastNoise->SetFractalOctaves(Octaves);
		FastNoise->SetFractalGain(Gain);
		FastNoise->SetFractalLacunarity(Lacunarity);
			
		/*FastNoise->SetCellularNoiseLookup(FastNoise);*/
		/*FastNoise->SetCellularDistanceFunction(ECellularDistanceFunction::Natural);
		FastNoise->SetCellularReturnType(ECellularReturnType::Distance2Sub);*/

		FastNoise->SetNoiseType(ENoiseType::Value);
		
		for (int32 TileX = 0; TileX < Rows; TileX++) {
			for (int32 TileY = 0; TileY < Columns; TileY++) {

				float Noise = (FastNoise->GetNoise((float)TileX, (float)TileY));

				if (Noise <= WaterLevel) {
					PlaceTile(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("water"));
				}

				if (Noise > WaterLevel) {
					PlaceTile(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("grass"));
				}

				UE_LOG(LogTemp, Warning, TEXT("Tile %dX %dY Layer %d, at %d pixels and %f Noise"), TileX, TileY, OceanLayer->GetLayerIndex(), TileSize, Noise);

			}
		}

		

		// Grow Island Seeds into full islands

		for (int32 TileX = 0; TileX < Rows; TileX++) {
			for (int32 TileY = 0; TileY < Columns; TileY++) {

				FString TileType = ExtractTileUserData(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("tiletype"));

				//UE_LOG(LogTemp, Warning, TEXT("Tile @ %dX %dY Layer %d is %s"), TileX, TileY, OceanLayer->GetLayerIndex(), *TileType);

				if (TileType.Equals(TEXT("grass"))) {
					

					int32 NorthY = TileY - 1;
					int32 SouthY = TileY + 1;
					int32 EastX = TileX - 1;
					int32 WestX = TileX + 1;

					//Place Tile North
					if (NorthY <= Rows) {
						PlaceTile(TileX, NorthY, GroundLayer->GetLayerIndex(), TEXT("grassN"));
						PlaceTile(TileX, NorthY, OceanLayer->GetLayerIndex(), TEXT("water"));
					}

					//Place Tile North East
					if (NorthY < Rows && EastX < Columns) {
						UE_LOG(LogTemp, Warning, TEXT("Found 'grass' @ %dX %dY Layer %d"), TileX, TileY, GroundLayer->GetLayerIndex());
						PlaceTile(EastX, NorthY, GroundLayer->GetLayerIndex(), TEXT("grassNE"));
						PlaceTile(EastX, NorthY, OceanLayer->GetLayerIndex(), TEXT("water"));
					}

					//Place Tile East

					//Place Tile South East

					//Place Tile South

					//Place Tile South West

					//Place Tile West

					//Place Tile North West

					PlaceTile(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("grass"));

				}

			}
		}

	}
	else {
		UE_LOG(LogTemp, Error, TEXT("ProceduralAnimatedTileMap must have a TileSet."))
	}

	BaseTileMap->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BaseTileMap->SetDefaultCollisionThickness(1000.0f);
	BaseTileMap->RebuildCollision();

}

void AProceduralAnimatedTileMap::PlaceTile(int32 TileX, int32 TileY, int32 LayerIndex, FString TileType) {
	TArray<int32> TileArray;
	TileTypes.MultiFind(TileType, TileArray, true);
	if (TileArray.Num() > 0) {
		FPaperTileInfo TileInfo = FPaperTileInfo();
		TileInfo.TileSet = BaseTileSet;
		int32 TileIndex = FMath::RandRange(0, TileArray.Num() - 1);
		TileInfo.PackedTileIndex = TileArray[TileIndex];
		BaseTileMap->SetTile(TileX, TileY, LayerIndex, TileInfo);
	}
}