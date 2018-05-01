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
	
	AProceduralAnimatedTileMap::PositionMapTopDown();

	AProceduralAnimatedTileMap::GenerateMap();

	AProceduralAnimatedTileMap::SetUpMapAnimation();

}

void AProceduralAnimatedTileMap::GenerateMap() {
	
	if (BaseTileSet) {

		CellNoise = NewObject<UFastNoise>(RootComponent, TEXT("CellGenerator"));
		FastNoise = NewObject<UFastNoise>(RootComponent, TEXT("NoiseGenerator"));

		BaseTileMap->MakeTileMapEditable();

		BaseTileMap->ResizeMap(Rows, Columns);

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


		//General Settings
		FastNoise->SetNoiseType(NoiseType);
		FastNoise->SetSeed(Seed);
		FastNoise->SetFrequency(Frequency);
		FastNoise->SetInterp(Interpolation);

		//Fractal Settings
		FastNoise->SetFractalType(FractalType);
		FastNoise->SetFractalOctaves(Octaves);
		FastNoise->SetFractalLacunarity(Lacunarity);
		FastNoise->SetFractalGain(Gain);
		
		//Cellular Settings
		FastNoise->SetCellularDistanceFunction(CellularDistance);
		FastNoise->SetCellularReturnType(CellularReturnType);
		//FastNoise->SetCellularNoiseLookup(CellNoise);

		// Warp Settings
		FastNoise->SetPositionWarpAmp(WarpAmp);
		FastNoise->SetPositionWarpType(WarpType);
		
		for (int32 TileX = 0; TileX < Rows; TileX++) {
			for (int32 TileY = 0; TileY < Columns; TileY++) {

				FVector2D Tile = FVector2D(TileX, TileY);

				float Noise = GetNoise(TileX, TileY);

				PlaceTile(TileX, TileY, OceanLayer->GetLayerIndex(), TEXT("water"));

				if (Noise >= WaterLevel) {

					TMap<FString, FVector2D> SurroundingGroundTiles;
					SurroundingGroundTiles.Add(TEXT("BeachTileN"), FVector2D(TileX, TileY - 1));
					SurroundingGroundTiles.Add(TEXT("BeachTileNE"), FVector2D(TileX + 1, TileY - 1));
					SurroundingGroundTiles.Add(TEXT("BeachTileE"), FVector2D(TileX + 1, TileY));
					SurroundingGroundTiles.Add(TEXT("BeachTileSE"), FVector2D(TileX + 1, TileY + 1));
					SurroundingGroundTiles.Add(TEXT("BeachTileS"), FVector2D(TileX, TileY + 1));
					SurroundingGroundTiles.Add(TEXT("BeachTileSW"), FVector2D(TileX - 1, TileY + 1));
					SurroundingGroundTiles.Add(TEXT("BeachTileW"), FVector2D(TileX - 1, TileY));
					SurroundingGroundTiles.Add(TEXT("BeachTileNW"), FVector2D(TileX - 1, TileY - 1));

					TMap<FString, FVector2D> SurroundingShallowsTiles;
					SurroundingShallowsTiles.Add(TEXT("ShallowsTileN"), FVector2D(TileX, TileY - 2));
					SurroundingShallowsTiles.Add(TEXT("ShallowsTileNxNE"), FVector2D(TileX + 1, TileY - 2));
					SurroundingShallowsTiles.Add(TEXT("ShallowsTileNE"), FVector2D(TileX + 2, TileY - 2));
					SurroundingShallowsTiles.Add(TEXT("ShallowsTileExNE"), FVector2D(TileX + 2, TileY + 1));
					SurroundingShallowsTiles.Add(TEXT("ShallowsTileE"), FVector2D(TileX + 2, TileY));
					SurroundingShallowsTiles.Add(TEXT("ShallowsTileExSE"), FVector2D(TileX + 2, TileY + 1));
					SurroundingShallowsTiles.Add(TEXT("ShallowsTileSE"), FVector2D(TileX + 2, TileY + 2));
					SurroundingShallowsTiles.Add(TEXT("ShallowsTileS"), FVector2D(TileX, TileY + 2));
					SurroundingShallowsTiles.Add(TEXT("ShallowsTileSxSW"), FVector2D(TileX - 1, TileY + 2));
					SurroundingShallowsTiles.Add(TEXT("ShallowsTileSW"), FVector2D(TileX - 2, TileY + 2));
					SurroundingShallowsTiles.Add(TEXT("ShallowsTileWxSW"), FVector2D(TileX - 2, TileY + 1));
					SurroundingShallowsTiles.Add(TEXT("ShallowsTileW"), FVector2D(TileX - 2, TileY));
					SurroundingShallowsTiles.Add(TEXT("ShallowsTileWxNW"), FVector2D(TileX - 2, TileY - 1));
					SurroundingShallowsTiles.Add(TEXT("ShallowsTileNW"), FVector2D(TileX - 2, TileY - 2));

					bool SuroundedByGround = true;
					TMap<FString, float> SurroundingTileNoise;
					for (auto& SGT : SurroundingGroundTiles)
					{

						float SGTNoise = AProceduralAnimatedTileMap::GetNoise(SGT.Value.X, SGT.Value.Y);
						
						SurroundingTileNoise.Add(SGT.Key, SGTNoise);
						
						if (SuroundedByGround) {
							if (SGTNoise < WaterLevel) {
								SuroundedByGround = false;
							}
						}
					}
					
					if (SuroundedByGround) {
						PlaceTile(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("grass"));
					}
					else {
						
						//Water On 4 Sides
						if (
							SurroundingTileNoise[TEXT("BeachTileN")] < WaterLevel &&
							SurroundingTileNoise[TEXT("BeachTileE")] < WaterLevel &&
							SurroundingTileNoise[TEXT("BeachTileS")] < WaterLevel &&
							SurroundingTileNoise[TEXT("BeachTileW")] < WaterLevel
						) {
							PlaceTile(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("sand"));
						}

						//Water On 3 Sides
						if (
							SurroundingTileNoise[TEXT("BeachTileE")] < WaterLevel &&
							SurroundingTileNoise[TEXT("BeachTileS")] < WaterLevel &&
							SurroundingTileNoise[TEXT("BeachTileW")] < WaterLevel
						) {
							PlaceTile(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("penisulaS"));
						} else if (
							SurroundingTileNoise[TEXT("BeachTileN")] < WaterLevel &&
							SurroundingTileNoise[TEXT("BeachTileS")] < WaterLevel &&
							SurroundingTileNoise[TEXT("BeachTileW")] < WaterLevel
						) {
							PlaceTile(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("penisulaW"));
						} else if (
							SurroundingTileNoise[TEXT("BeachTileN")] < WaterLevel &&
							SurroundingTileNoise[TEXT("BeachTileE")] < WaterLevel &&
							SurroundingTileNoise[TEXT("BeachTileW")] < WaterLevel
						) {
							PlaceTile(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("penisulaN"));
						} else if (
							SurroundingTileNoise[TEXT("BeachTileN")] < WaterLevel &&
							SurroundingTileNoise[TEXT("BeachTileE")] < WaterLevel &&
							SurroundingTileNoise[TEXT("BeachTileS")] < WaterLevel
						) {
							PlaceTile(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("penisulaE"));
						}

						//Water On 2 Sides
						else if (
							SurroundingTileNoise[TEXT("BeachTileN")] < WaterLevel &&
							SurroundingTileNoise[TEXT("BeachTileS")] < WaterLevel
						) {
							PlaceTile(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("isthmusEW"));
						}
						else if (
							SurroundingTileNoise[TEXT("BeachTileN")] < WaterLevel &&
							SurroundingTileNoise[TEXT("BeachTileW")] < WaterLevel
							) {
							PlaceTile(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("beachNW"));
						}
						else if (
							SurroundingTileNoise[TEXT("BeachTileN")] < WaterLevel &&
							SurroundingTileNoise[TEXT("BeachTileE")] < WaterLevel
							) {
							PlaceTile(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("beachNE"));
						}
						else if (
							SurroundingTileNoise[TEXT("BeachTileS")] < WaterLevel &&
							SurroundingTileNoise[TEXT("BeachTileE")] < WaterLevel
							) {
							PlaceTile(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("beachSE"));
						}
						else if (
							SurroundingTileNoise[TEXT("BeachTileS")] < WaterLevel &&
							SurroundingTileNoise[TEXT("BeachTileW")] < WaterLevel
							) {
							PlaceTile(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("beachSW"));
						}
						else if (
							SurroundingTileNoise[TEXT("BeachTileW")] < WaterLevel &&
							SurroundingTileNoise[TEXT("BeachTileE")] < WaterLevel
							) {
							PlaceTile(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("isthmusNS"));
						}

						//Water on 1 Side
						else if (SurroundingTileNoise[TEXT("BeachTileN")] < WaterLevel) {
							PlaceTile(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("beachN"));
						}
						else if (SurroundingTileNoise[TEXT("BeachTileE")] < WaterLevel) {
							PlaceTile(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("beachE"));
						}
						else if (SurroundingTileNoise[TEXT("BeachTileS")] < WaterLevel) {
							PlaceTile(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("beachS"));
						}
						else if (SurroundingTileNoise[TEXT("BeachTileW")] < WaterLevel) {
							PlaceTile(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("beachW"));
						}

						// 4 Diagnals are water
						else if (
							SurroundingTileNoise[TEXT("BeachTileNE")] < WaterLevel &&
							SurroundingTileNoise[TEXT("BeachTileSE")] < WaterLevel &&
							SurroundingTileNoise[TEXT("BeachTileSW")] < WaterLevel &&
							SurroundingTileNoise[TEXT("BeachTileNW")] < WaterLevel
						) {
							PlaceTile(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("sand"));
						}

						// Three diagonals are water
						else if (
							SurroundingTileNoise[TEXT("BeachTileNE")] < WaterLevel &&
							SurroundingTileNoise[TEXT("BeachTileSE")] < WaterLevel &&
							SurroundingTileNoise[TEXT("BeachTileSW")] < WaterLevel
						) {
							PlaceTile(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("sand"));
						}
						else if (
							SurroundingTileNoise[TEXT("BeachTileSE")] < WaterLevel &&
							SurroundingTileNoise[TEXT("BeachTileSW")] < WaterLevel &&
							SurroundingTileNoise[TEXT("BeachTileNW")] < WaterLevel
							) {
							PlaceTile(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("sand"));
						}
						else if (
							SurroundingTileNoise[TEXT("BeachTileSW")] < WaterLevel &&
							SurroundingTileNoise[TEXT("BeachTileNW")] < WaterLevel &&
							SurroundingTileNoise[TEXT("BeachTileNE")] < WaterLevel
							) {
							PlaceTile(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("sand"));
						}
						else if (
							SurroundingTileNoise[TEXT("BeachTileNW")] < WaterLevel &&
							SurroundingTileNoise[TEXT("BeachTileNE")] < WaterLevel &&
							SurroundingTileNoise[TEXT("BeachTileSE")] < WaterLevel
						) {
							PlaceTile(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("sand"));
						}

						//// Two diagonals are water
						else if (
							SurroundingTileNoise[TEXT("BeachTileNE")] < WaterLevel &&
							SurroundingTileNoise[TEXT("BeachTileSE")] < WaterLevel
						) {
							PlaceTile(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("sand"));
						}
						else if (
							SurroundingTileNoise[TEXT("BeachTileSE")] < WaterLevel &&
							SurroundingTileNoise[TEXT("BeachTileSW")] < WaterLevel
							) {
							PlaceTile(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("sand"));
						}
						else if (
							SurroundingTileNoise[TEXT("BeachTileSW")] < WaterLevel &&
							SurroundingTileNoise[TEXT("BeachTileNW")] < WaterLevel
							) {
							PlaceTile(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("sand"));
						}
						else if (
							SurroundingTileNoise[TEXT("BeachTileNW")] < WaterLevel &&
							SurroundingTileNoise[TEXT("BeachTileSE")] < WaterLevel
							) {
							PlaceTile(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("sand"));
						}

						//One diagonal is water
						else if (
							SurroundingTileNoise[TEXT("BeachTileNE")] < WaterLevel
						) {
							PlaceTile(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("beachCornerNE"));
						}
						else if (
							SurroundingTileNoise[TEXT("BeachTileSE")] < WaterLevel
						) {
							PlaceTile(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("beachCornerSE"));
						}
						else if (
							SurroundingTileNoise[TEXT("BeachTileSW")] < WaterLevel
						) {
							PlaceTile(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("beachCornerSW"));
						}
						else if (
							SurroundingTileNoise[TEXT("BeachTileNW")] < WaterLevel 
						) {
							PlaceTile(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("beachCornerNW"));
						}
					}
				
				}

				UE_LOG(LogTemp, Warning, TEXT("Tile %dX %dY Layer %d, at %d pixels and %f Noise"), TileX, TileY, OceanLayer->GetLayerIndex(), TileSize, Noise);

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

float AProceduralAnimatedTileMap::GetNoise(int32 X, int32 Y) {
	return (FastNoise->GetNoise(X, Y)) * 1000.f;
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