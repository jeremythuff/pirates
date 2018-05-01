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

				float Noise = (AProceduralAnimatedTileMap::FastNoise->GetNoise((float)TileX, (float)TileY)) *1000.f;

				PlaceTile(TileX, TileY, OceanLayer->GetLayerIndex(), TEXT("water"));

				if (Noise >= WaterLevel) {
					PlaceTile(TileX, TileY, GroundLayer->GetLayerIndex(), TEXT("grass"));
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