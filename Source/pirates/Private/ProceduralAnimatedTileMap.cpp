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
		
}

// Called when the game starts or when spawned
void AProceduralAnimatedTileMap::BeginPlay()
{
	
	AProceduralAnimatedTileMap::PositionMapTopDown();

	AProceduralAnimatedTileMap::GenerateMap();

	AProceduralAnimatedTileMap::SetUpMapAnimation();

}

void AProceduralAnimatedTileMap::GenerateMap() {
	
	if (BaseTileSet) {

		FastNoise = NewObject<UFastNoise>(RootComponent, TEXT("NoiseGenerator"));

		BaseTileMap->MakeTileMapEditable();

		BaseTileMap->ResizeMap(Rows, Columns);

		//TODO: Not working...
		BaseTileMap->TileMap->SeparationPerLayer = 5.0f;

		UPaperTileLayer* FortaficationsLayer = BaseTileMap->TileMap->AddNewLayer();
		FortaficationsLayer->SetLayerCollides(true);

		UPaperTileLayer* FoliageLayer = BaseTileMap->TileMap->AddNewLayer();
		FoliageLayer->SetLayerCollides(false);

		UPaperTileLayer* GroundLayer = BaseTileMap->TileMap->AddNewLayer();
		GroundLayer->SetLayerCollides(true);

		UPaperTileLayer* ShallowsLayer = BaseTileMap->TileMap->AddNewLayer();

		FLinearColor newColor = FLinearColor(ShallowsLayer->GetLayerColor());
		newColor.A = 0.9f;
		ShallowsLayer->SetLayerColor(newColor);
		ShallowsLayer->SetLayerCollides(false);

		UPaperTileLayer* OceanLayer = BaseTileMap->TileMap->AddNewLayer();
		OceanLayer->SetLayerCollides(false);

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
					PlaceTile(TileX, TileY, OceanLayer->GetLayerIndex(), TEXT("water"));
				}

				if (Noise > WaterLevel) {
					PlaceTile(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("grass"));
				}

				UE_LOG(LogTemp, Warning, TEXT("Tile %dX %dY Layer %d, at %d pixels and %f Noise"), TileX, TileY, OceanLayer->GetLayerIndex(), TileSize, Noise);

			}
		}

		

		// Grow Island Seeds into full islands

		//Build Coastlines
		for (int32 TileX = 0; TileX < Rows; TileX++) {
			for (int32 TileY = 0; TileY < Columns; TileY++) {

				FString TileType = ExtractTileUserData(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("tiletype"));

				//UE_LOG(LogTemp, Warning, TEXT("Tile @ %dX %dY Layer %d is %s"), TileX, TileY, OceanLayer->GetLayerIndex(), *TileType);

				if (TileType.Equals(TEXT("grass"))) {
					

					int32 BeachNorthY = TileY - 1;
					int32 BeachSouthY = TileY + 1;
					int32 BeachEastX = TileX + 1;
					int32 BeachWestX = TileX - 1;

					//Place Beach North
					if (BeachNorthY < Rows) {
						PlaceTile(TileX, BeachNorthY, GroundLayer->GetLayerIndex(), TEXT("grassN"));
					}

					//Place Beach North East
					if (BeachNorthY < Rows && BeachEastX < Columns) {
						PlaceTile(BeachEastX, BeachNorthY, GroundLayer->GetLayerIndex(), TEXT("grassNE"));
					}

					//Place Beach East
					if (BeachEastX < Columns) {
						PlaceTile(BeachEastX, TileY, GroundLayer->GetLayerIndex(), TEXT("grassE"));
					}

					//Place Beach South East
					if (BeachSouthY >= 0 && BeachEastX < Columns) {
						PlaceTile(BeachEastX, BeachSouthY, GroundLayer->GetLayerIndex(), TEXT("grassSE"));
					}

					//Place Beach South
					if (BeachSouthY >= 0) {
						PlaceTile(TileX, BeachSouthY, GroundLayer->GetLayerIndex(), TEXT("grassS"));
					}

					//Place Beach South West
					if (BeachSouthY >= 0 && BeachWestX >= 0) {
						PlaceTile(BeachWestX, BeachSouthY, GroundLayer->GetLayerIndex(), TEXT("grassSW"));
					}

					//Place Beach West
					if (BeachWestX >= 0) {
						PlaceTile(BeachWestX, TileY, GroundLayer->GetLayerIndex(), TEXT("grassW"));
					}

					//Place Beach North West
					if (BeachNorthY < Rows && BeachWestX >= 0) {
						PlaceTile(BeachWestX, BeachNorthY, GroundLayer->GetLayerIndex(), TEXT("grassNW"));
					}

					int32 ShallowsNorthY = TileY - 2;
					int32 ShallowsSouthY = TileY + 2;
					int32 ShallowsEastX = TileX + 2;
					int32 ShallowsWestX = TileX - 2;

					//Place Shallows North
					if (ShallowsNorthY < Rows) {
						PlaceTile(TileX, ShallowsNorthY, ShallowsLayer->GetLayerIndex(), TEXT("shallowsN"));
						PlaceTile(TileX, BeachNorthY, ShallowsLayer->GetLayerIndex(), TEXT("shallows"));
						


						PlaceTile(BeachWestX, ShallowsNorthY, ShallowsLayer->GetLayerIndex(), TEXT("shallowsN"));
						PlaceTile(BeachWestX, BeachNorthY, ShallowsLayer->GetLayerIndex(), TEXT("shallows"));
						

						PlaceTile(BeachEastX, ShallowsNorthY, ShallowsLayer->GetLayerIndex(), TEXT("shallowsN"));
						PlaceTile(BeachEastX, BeachNorthY, ShallowsLayer->GetLayerIndex(), TEXT("shallows"));

					}

					//Place Shallows North East
					if (ShallowsNorthY < Rows && ShallowsEastX < Columns) {
						PlaceTile(ShallowsEastX, ShallowsNorthY, ShallowsLayer->GetLayerIndex(), TEXT("shallowsNE"));
					}

					//Place Shallows East
					if (ShallowsEastX < Columns) {
						PlaceTile(ShallowsEastX, TileY, ShallowsLayer->GetLayerIndex(), TEXT("shallowsE"));
						PlaceTile(BeachEastX, TileY, ShallowsLayer->GetLayerIndex(), TEXT("shallows"));



						PlaceTile(ShallowsEastX, BeachNorthY, ShallowsLayer->GetLayerIndex(), TEXT("shallowsE"));
						PlaceTile(BeachEastX, BeachNorthY, ShallowsLayer->GetLayerIndex(), TEXT("shallows"));


						PlaceTile(ShallowsEastX, BeachSouthY, ShallowsLayer->GetLayerIndex(), TEXT("shallowsE"));
						PlaceTile(BeachEastX, BeachSouthY, ShallowsLayer->GetLayerIndex(), TEXT("shallows"));
					}

					//Place Shallows South East
					if (ShallowsSouthY >= 0 && ShallowsEastX < Columns) {
						PlaceTile(ShallowsEastX, ShallowsSouthY, ShallowsLayer->GetLayerIndex(), TEXT("shallowsSE"));
					}

					//Place Shallows South
					if (ShallowsSouthY >= 0) {
						PlaceTile(TileX, ShallowsSouthY, ShallowsLayer->GetLayerIndex(), TEXT("shallowsS"));
						PlaceTile(TileX, BeachSouthY, ShallowsLayer->GetLayerIndex(), TEXT("shallows"));



						PlaceTile(BeachWestX, ShallowsSouthY, ShallowsLayer->GetLayerIndex(), TEXT("shallowsS"));
						PlaceTile(BeachWestX, BeachSouthY, ShallowsLayer->GetLayerIndex(), TEXT("shallows"));


						PlaceTile(BeachEastX, ShallowsSouthY, ShallowsLayer->GetLayerIndex(), TEXT("shallowsS"));
						PlaceTile(BeachEastX, BeachSouthY, ShallowsLayer->GetLayerIndex(), TEXT("shallows"));
					}

					//Place Shallows South West
					if (ShallowsSouthY >= 0 && ShallowsWestX >= 0) {
						PlaceTile(ShallowsWestX, ShallowsSouthY, ShallowsLayer->GetLayerIndex(), TEXT("shallowsSW"));
					}

					//Place Shallows West
					if (ShallowsWestX >= 0) {
						PlaceTile(ShallowsWestX, TileY, ShallowsLayer->GetLayerIndex(), TEXT("shallowsW"));
						PlaceTile(BeachWestX, TileY, ShallowsLayer->GetLayerIndex(), TEXT("shallows"));

						PlaceTile(ShallowsWestX, BeachNorthY, ShallowsLayer->GetLayerIndex(), TEXT("shallowsW"));
						PlaceTile(BeachWestX, BeachNorthY, ShallowsLayer->GetLayerIndex(), TEXT("shallows"));

						PlaceTile(ShallowsWestX, BeachSouthY, ShallowsLayer->GetLayerIndex(), TEXT("shallowsW"));
						PlaceTile(BeachWestX, BeachSouthY, ShallowsLayer->GetLayerIndex(), TEXT("shallows"));
					}

					//Place Shallows North West
					if (ShallowsNorthY < Rows && ShallowsWestX >= 0) {
						PlaceTile(ShallowsWestX, ShallowsNorthY, ShallowsLayer->GetLayerIndex(), TEXT("shallowsNW"));
					}


				}

			}
		}

	}
	else {
		UE_LOG(LogTemp, Error, TEXT("ProceduralAnimatedTileMap must have a TileSet."))
	}

	BaseTileMap->TileMap->SetCollisionDomain(ESpriteCollisionMode::Use3DPhysics);
	BaseTileMap->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BaseTileMap->SetDefaultCollisionThickness(1000.0f);
	BaseTileMap->RebuildCollision();
}

void AProceduralAnimatedTileMap::PositionMapTopDown() {

	if (BaseTileSet) {

		float MapWidth = (float)Columns*(float)BaseTileSet->GetTileSize().X;
		float MapHeight = (float)Rows*(float)BaseTileSet->GetTileSize().Y;

		BaseTileMap->SetRelativeRotation(FRotator(0.0f, 90.0f, -90.0f));
		BaseTileMap->SetRelativeLocation(FVector(MapWidth, (MapWidth / 2.0f)*(-1.0f), 0.0f));

	}

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