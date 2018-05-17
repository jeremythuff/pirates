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

		TMap<FVector2D, float> noise;
		Noise = noise;

		for (int32 TileX = 0; TileX < Rows; TileX++) {
			for (int32 TileY = 0; TileY < Columns; TileY++) {
				AProceduralAnimatedTileMap::Noise.Add(FVector2D(TileX, TileY), (FastNoise->GetNoise(TileX, TileY)) * 1000.f);
			}
		}

		float ShallowsStart = WaterLevel;
		float ShallowsStop = ShallowsStart + ShallowsThickness;
		float GrassStart = ShallowsStop;

		for (auto& Entry : Noise)
		{
			FVector2D Tile = Entry.Key;

			PlaceTile(Tile.X, Tile.Y, OceanLayer->GetLayerIndex(), TEXT("water"));

			if (Entry.Value > ShallowsStart && Entry.Value <= ShallowsStop) {
				FString TileName = SelectTileName(Tile, ShallowsStart, TEXT("shallows"));
				PlaceTile(Tile.X, Tile.Y, ShallowsLayer->GetLayerIndex(), TileName);
			}
			else if (Entry.Value > GrassStart) {
				FString TileName = SelectTileName(Tile, GrassStart - 1.0f, TEXT("grass"));
				PlaceTile(Tile.X, Tile.Y, GroundLayer->GetLayerIndex(), TileName);
				FString STileName = SelectTileName(Tile, GrassStart - 1.0f, TEXT("grass"));
				PlaceTile(Tile.X, Tile.Y, ShallowsLayer->GetLayerIndex(), STileName);
			}

			UE_LOG(LogTemp, Warning, TEXT("Tile %dX %dY Layer %d, at %d pixels and %f Noise"), Tile.X, Tile.Y, OceanLayer->GetLayerIndex(), TileSize, Entry.Value);
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

FString AProceduralAnimatedTileMap::SelectTileName(FVector2D Tile, float Threshold, FString NameRoot)
{
	TMap<FString, bool> TileTouching = AProceduralAnimatedTileMap::SidesTouching(Tile, Threshold);
	UE_LOG(LogTemp, Warning, TEXT("NameRoot = %s, Threshold = %f"), *NameRoot, Threshold, (TileTouching["N"] ? TEXT("True") : TEXT("False")));
	// Eight Sides
	if (
		TileTouching[TEXT("N")] &&
		TileTouching[TEXT("NE")] &&
		TileTouching[TEXT("E")] &&
		TileTouching[TEXT("SE")] &&
		TileTouching[TEXT("S")] &&
		TileTouching[TEXT("SW")] &&
		TileTouching[TEXT("W")] &&
		TileTouching[TEXT("NW")]
	) {
		//TODO: Create a solo tile single circle.
	}

	// 7 Sides

	// 6 Sides

	// 5 Sides

	else if (
		TileTouching[TEXT("N")] &&
		TileTouching[TEXT("NE")] &&
		TileTouching[TEXT("E")] &&
		TileTouching[TEXT("SE")] &&
		!TileTouching[TEXT("S")] &&
		!TileTouching[TEXT("SW")] &&
		!TileTouching[TEXT("W")] &&
		TileTouching[TEXT("NW")]
	) {
		NameRoot.Append(TEXT("NE"));
	}

	else if (
		!TileTouching[TEXT("N")] &&
		TileTouching[TEXT("NE")] &&
		TileTouching[TEXT("E")] &&
		TileTouching[TEXT("SE")] &&
		TileTouching[TEXT("S")] &&
		TileTouching[TEXT("SW")] &&
		!TileTouching[TEXT("W")] &&
		!TileTouching[TEXT("NW")]
	) {
		NameRoot.Append(TEXT("SE"));
	}

	else if (
		!TileTouching[TEXT("N")] &&
		!TileTouching[TEXT("NE")] &&
		!TileTouching[TEXT("E")] &&
		TileTouching[TEXT("SE")] &&
		TileTouching[TEXT("S")] &&
		TileTouching[TEXT("SW")] &&
		TileTouching[TEXT("W")] &&
		TileTouching[TEXT("NW")]
	) {
		NameRoot.Append(TEXT("SW"));
	}

	else if (
		TileTouching[TEXT("N")] &&
		TileTouching[TEXT("NE")] &&
		!TileTouching[TEXT("E")] &&
		!TileTouching[TEXT("SE")] &&
		!TileTouching[TEXT("S")] &&
		TileTouching[TEXT("SW")] &&
		TileTouching[TEXT("W")] &&
		TileTouching[TEXT("NW")]
	) {
		NameRoot.Append(TEXT("NW"));
	}

	// 4 Sides

	else if (
		TileTouching[TEXT("N")] &&
		TileTouching[TEXT("NE")] &&
		TileTouching[TEXT("E")] &&
		!TileTouching[TEXT("SE")] &&
		!TileTouching[TEXT("S")] &&
		!TileTouching[TEXT("SW")] &&
		!TileTouching[TEXT("W")] &&
		TileTouching[TEXT("NW")]
	) {
		NameRoot.Append(TEXT("NE"));
	}
	else if (
		TileTouching[TEXT("N")] &&
		TileTouching[TEXT("NE")] &&
		TileTouching[TEXT("E")] &&
		TileTouching[TEXT("SE")] &&
		!TileTouching[TEXT("S")] &&
		!TileTouching[TEXT("SW")] &&
		!TileTouching[TEXT("W")] &&
		!TileTouching[TEXT("NW")]
		) {
		NameRoot.Append(TEXT("NE"));
	}


	else if (
		!TileTouching[TEXT("N")] &&
		TileTouching[TEXT("NE")] &&
		TileTouching[TEXT("E")] &&
		TileTouching[TEXT("SE")] &&
		TileTouching[TEXT("S")] &&
		!TileTouching[TEXT("SW")] &&
		!TileTouching[TEXT("W")] &&
		!TileTouching[TEXT("NW")]
	) {
		NameRoot.Append(TEXT("SE"));
	}

	else if (
		!TileTouching[TEXT("N")] &&
		!TileTouching[TEXT("NE")] &&
		TileTouching[TEXT("E")] &&
		TileTouching[TEXT("SE")] &&
		TileTouching[TEXT("S")] &&
		TileTouching[TEXT("SW")] &&
		!TileTouching[TEXT("W")] &&
		!TileTouching[TEXT("NW")]
		) {
		NameRoot.Append(TEXT("SE"));
	}

	else if (
		!TileTouching[TEXT("N")] &&
		!TileTouching[TEXT("NE")] &&
		!TileTouching[TEXT("E")] &&
		TileTouching[TEXT("SE")] &&
		TileTouching[TEXT("S")] &&
		TileTouching[TEXT("SW")] &&
		TileTouching[TEXT("W")] &&
		!TileTouching[TEXT("NW")]
	) {
		NameRoot.Append(TEXT("SW"));
	}

	else if (
		!TileTouching[TEXT("N")] &&
		!TileTouching[TEXT("NE")] &&
		!TileTouching[TEXT("E")] &&
		!TileTouching[TEXT("SE")] &&
		TileTouching[TEXT("S")] &&
		TileTouching[TEXT("SW")] &&
		TileTouching[TEXT("W")] &&
		TileTouching[TEXT("NW")]
		) {
		NameRoot.Append(TEXT("SW"));
	}

	else if (
		TileTouching[TEXT("N")] &&
		!TileTouching[TEXT("NE")] &&
		!TileTouching[TEXT("E")] &&
		!TileTouching[TEXT("SE")] &&
		!TileTouching[TEXT("S")] &&
		TileTouching[TEXT("SW")] &&
		TileTouching[TEXT("W")] &&
		TileTouching[TEXT("NW")]
	) {
		NameRoot.Append(TEXT("NW"));
	}

	else if (
		TileTouching[TEXT("N")] &&
		TileTouching[TEXT("NE")] &&
		!TileTouching[TEXT("E")] &&
		!TileTouching[TEXT("SE")] &&
		!TileTouching[TEXT("S")] &&
		!TileTouching[TEXT("SW")] &&
		TileTouching[TEXT("W")] &&
		TileTouching[TEXT("NW")]
	) {
		NameRoot.Append(TEXT("NW"));
	}

	// 3 Sides 

	else if (
		TileTouching[TEXT("N")] &&
		TileTouching[TEXT("NE")] &&
		!TileTouching[TEXT("E")] &&
		!TileTouching[TEXT("SE")] &&
		!TileTouching[TEXT("S")] &&
		!TileTouching[TEXT("SW")] &&
		!TileTouching[TEXT("W")] &&
		TileTouching[TEXT("NW")]
	) {
		NameRoot.Append(TEXT("N"));
	}

	else if (
		TileTouching[TEXT("N")] &&
		TileTouching[TEXT("NE")] &&
		TileTouching[TEXT("E")] &&
		!TileTouching[TEXT("SE")] &&
		!TileTouching[TEXT("S")] &&
		!TileTouching[TEXT("SW")] &&
		!TileTouching[TEXT("W")] &&
		!TileTouching[TEXT("NW")]
		) {
		NameRoot.Append(TEXT("NE"));
	}

	else if (
		!TileTouching[TEXT("N")] &&
		TileTouching[TEXT("NE")] &&
		TileTouching[TEXT("E")] &&
		TileTouching[TEXT("SE")] &&
		!TileTouching[TEXT("S")] &&
		!TileTouching[TEXT("SW")] &&
		!TileTouching[TEXT("W")] &&
		!TileTouching[TEXT("NW")]
	) {
		NameRoot.Append(TEXT("E"));
	}

	else if (
		!TileTouching[TEXT("N")] &&
		!TileTouching[TEXT("NE")] &&
		TileTouching[TEXT("E")] &&
		TileTouching[TEXT("SE")] &&
		TileTouching[TEXT("S")] &&
		!TileTouching[TEXT("SW")] &&
		!TileTouching[TEXT("W")] &&
		!TileTouching[TEXT("NW")]
	) {
		NameRoot.Append(TEXT("SE"));
	}

	else if (
		!TileTouching[TEXT("N")] &&
		!TileTouching[TEXT("NE")] &&
		!TileTouching[TEXT("E")] &&
		TileTouching[TEXT("SE")] &&
		TileTouching[TEXT("S")] &&
		TileTouching[TEXT("SW")] &&
		!TileTouching[TEXT("W")] &&
		!TileTouching[TEXT("NW")]
	) {
		NameRoot.Append(TEXT("S"));
	}

	else if (
		!TileTouching[TEXT("N")] &&
		!TileTouching[TEXT("NE")] &&
		!TileTouching[TEXT("E")] &&
		!TileTouching[TEXT("SE")] &&
		TileTouching[TEXT("S")] &&
		TileTouching[TEXT("SW")] &&
		TileTouching[TEXT("W")] &&
		!TileTouching[TEXT("NW")]
	) {
		NameRoot.Append(TEXT("SW"));
	}

	else if (
		!TileTouching[TEXT("N")] &&
		!TileTouching[TEXT("NE")] &&
		!TileTouching[TEXT("E")] &&
		!TileTouching[TEXT("SE")] &&
		!TileTouching[TEXT("S")] &&
		TileTouching[TEXT("SW")] &&
		TileTouching[TEXT("W")] &&
		TileTouching[TEXT("NW")]
	) {
		NameRoot.Append(TEXT("W"));
	}

	else if (
		TileTouching[TEXT("N")] &&
		!TileTouching[TEXT("NE")] &&
		!TileTouching[TEXT("E")] &&
		!TileTouching[TEXT("SE")] &&
		!TileTouching[TEXT("S")] &&
		!TileTouching[TEXT("SW")] &&
		TileTouching[TEXT("W")] &&
		TileTouching[TEXT("NW")]
	) {
		NameRoot.Append(TEXT("NW"));
	}


	// 2 Sides

	else if (
		TileTouching[TEXT("N")] &&
		TileTouching[TEXT("NE")] &&
		!TileTouching[TEXT("E")] &&
		!TileTouching[TEXT("SE")] &&
		!TileTouching[TEXT("S")] &&
		!TileTouching[TEXT("SW")] &&
		!TileTouching[TEXT("W")] &&
		!TileTouching[TEXT("NW")]
	) {
		NameRoot.Append(TEXT("N"));
	}

	else if (
		!TileTouching[TEXT("N")] &&
		TileTouching[TEXT("NE")] &&
		TileTouching[TEXT("E")] &&
		!TileTouching[TEXT("SE")] &&
		!TileTouching[TEXT("S")] &&
		!TileTouching[TEXT("SW")] &&
		!TileTouching[TEXT("W")] &&
		!TileTouching[TEXT("NW")]
		) {
		NameRoot.Append(TEXT("E"));
	}

	else if (
		!TileTouching[TEXT("N")] &&
		!TileTouching[TEXT("NE")] &&
		TileTouching[TEXT("E")] &&
		TileTouching[TEXT("SE")] &&
		!TileTouching[TEXT("S")] &&
		!TileTouching[TEXT("SW")] &&
		!TileTouching[TEXT("W")] &&
		!TileTouching[TEXT("NW")]
		) {
		NameRoot.Append(TEXT("E"));
	}

	else if (
		!TileTouching[TEXT("N")] &&
		!TileTouching[TEXT("NE")] &&
		!TileTouching[TEXT("E")] &&
		TileTouching[TEXT("SE")] &&
		TileTouching[TEXT("S")] &&
		!TileTouching[TEXT("SW")] &&
		!TileTouching[TEXT("W")] &&
		!TileTouching[TEXT("NW")]
	) {
		NameRoot.Append(TEXT("S"));
	}

	else if (
		!TileTouching[TEXT("N")] &&
		!TileTouching[TEXT("NE")] &&
		!TileTouching[TEXT("E")] &&
		!TileTouching[TEXT("SE")] &&
		TileTouching[TEXT("S")] &&
		TileTouching[TEXT("SW")] &&
		!TileTouching[TEXT("W")] &&
		!TileTouching[TEXT("NW")]
	) {
		NameRoot.Append(TEXT("S"));
	}

	else if (
		!TileTouching[TEXT("N")] &&
		!TileTouching[TEXT("NE")] &&
		!TileTouching[TEXT("E")] &&
		!TileTouching[TEXT("SE")] &&
		!TileTouching[TEXT("S")] &&
		TileTouching[TEXT("SW")] &&
		TileTouching[TEXT("W")] &&
		!TileTouching[TEXT("NW")]
	) {
		NameRoot.Append(TEXT("W"));
	}

	else if (
		!TileTouching[TEXT("N")] &&
		!TileTouching[TEXT("NE")] &&
		!TileTouching[TEXT("E")] &&
		!TileTouching[TEXT("SE")] &&
		!TileTouching[TEXT("S")] &&
		!TileTouching[TEXT("SW")] &&
		TileTouching[TEXT("W")] &&
		TileTouching[TEXT("NW")]
	) {
		NameRoot.Append(TEXT("W"));
	}

	else if (
		TileTouching[TEXT("N")] &&
		!TileTouching[TEXT("NE")] &&
		!TileTouching[TEXT("E")] &&
		!TileTouching[TEXT("SE")] &&
		!TileTouching[TEXT("S")] &&
		!TileTouching[TEXT("SW")] &&
		!TileTouching[TEXT("W")] &&
		TileTouching[TEXT("NW")]
	) {
		NameRoot.Append(TEXT("N"));
	}

	// 1 Side

	else if (
		TileTouching[TEXT("N")] &&
		!TileTouching[TEXT("NE")] &&
		!TileTouching[TEXT("E")] &&
		!TileTouching[TEXT("SE")] &&
		!TileTouching[TEXT("S")] &&
		!TileTouching[TEXT("SW")] &&
		!TileTouching[TEXT("W")] &&
		!TileTouching[TEXT("NW")]
		) {
		NameRoot.Append(TEXT("N"));
	}
	
	else if (
		!TileTouching[TEXT("N")] &&
		TileTouching[TEXT("NE")] &&
		!TileTouching[TEXT("E")] &&
		!TileTouching[TEXT("SE")] &&
		!TileTouching[TEXT("S")] &&
		!TileTouching[TEXT("SW")] &&
		!TileTouching[TEXT("W")] &&
		!TileTouching[TEXT("NW")]
		) {
		NameRoot.Append(TEXT("CNE"));
	}

	else if (
		!TileTouching[TEXT("N")] &&
		!TileTouching[TEXT("NE")] &&
		TileTouching[TEXT("E")] &&
		!TileTouching[TEXT("SE")] &&
		!TileTouching[TEXT("S")] &&
		!TileTouching[TEXT("SW")] &&
		!TileTouching[TEXT("W")] &&
		!TileTouching[TEXT("NW")]
		) {
		NameRoot.Append(TEXT("E"));
	}

	else if (
		!TileTouching[TEXT("N")] &&
		!TileTouching[TEXT("NE")] &&
		!TileTouching[TEXT("E")] &&
		TileTouching[TEXT("SE")] &&
		!TileTouching[TEXT("S")] &&
		!TileTouching[TEXT("SW")] &&
		!TileTouching[TEXT("W")] &&
		!TileTouching[TEXT("NW")]
		) {
		NameRoot.Append(TEXT("CSE"));
	}

	else if (
		!TileTouching[TEXT("N")] &&
		!TileTouching[TEXT("NE")] &&
		!TileTouching[TEXT("E")] &&
		!TileTouching[TEXT("SE")] &&
		TileTouching[TEXT("S")] &&
		!TileTouching[TEXT("SW")] &&
		!TileTouching[TEXT("W")] &&
		!TileTouching[TEXT("NW")]
		) {
		NameRoot.Append(TEXT("S"));
	}

	else if (
		!TileTouching[TEXT("N")] &&
		!TileTouching[TEXT("NE")] &&
		!TileTouching[TEXT("E")] &&
		!TileTouching[TEXT("SE")] &&
		!TileTouching[TEXT("S")] &&
		TileTouching[TEXT("SW")] &&
		!TileTouching[TEXT("W")] &&
		!TileTouching[TEXT("NW")]
		) {
		NameRoot.Append(TEXT("CSW"));
	}

	else if (
		!TileTouching[TEXT("N")] &&
		!TileTouching[TEXT("NE")] &&
		!TileTouching[TEXT("E")] &&
		!TileTouching[TEXT("SE")] &&
		!TileTouching[TEXT("S")] &&
		!TileTouching[TEXT("SW")] &&
		TileTouching[TEXT("W")] &&
		!TileTouching[TEXT("NW")]
		) {
		NameRoot.Append(TEXT("W"));
	}

	else if (
		!TileTouching[TEXT("N")] &&
		!TileTouching[TEXT("NE")] &&
		!TileTouching[TEXT("E")] &&
		!TileTouching[TEXT("SE")] &&
		!TileTouching[TEXT("S")] &&
		!TileTouching[TEXT("SW")] &&
		!TileTouching[TEXT("W")] &&
		TileTouching[TEXT("NW")]
		) {
		NameRoot.Append(TEXT("CNW"));
	}
	return NameRoot;
}

bool AProceduralAnimatedTileMap::TileIsTouching(FVector2D Tile, float Threshold)
{
	TMap<FString, bool> TileTouching = AProceduralAnimatedTileMap::SidesTouching(Tile, Threshold);
	TArray<bool> Vals;
	TileTouching.GenerateValueArray(Vals);
	return Vals.Contains(true);
}

TMap<FString, bool> AProceduralAnimatedTileMap::SidesTouching(FVector2D Tile, float Threshold) {
	float TileNoiseN = AProceduralAnimatedTileMap::GetNoise(Tile.X, Tile.Y - 1);
	float TileNoiseNE = AProceduralAnimatedTileMap::GetNoise(Tile.X + 1, Tile.Y - 1);
	float TileNoiseE = AProceduralAnimatedTileMap::GetNoise(Tile.X + 1, Tile.Y);
	float TileNoiseSE = AProceduralAnimatedTileMap::GetNoise(Tile.X + 1, Tile.Y + 1);
	float TileNoiseS = AProceduralAnimatedTileMap::GetNoise(Tile.X, Tile.Y + 1);
	float TileNoiseSW = AProceduralAnimatedTileMap::GetNoise(Tile.X - 1, Tile.Y + 1);
	float TileNoiseW = AProceduralAnimatedTileMap::GetNoise(Tile.X - 1, Tile.Y);
	float TileNoiseNW = AProceduralAnimatedTileMap::GetNoise(Tile.X - 1, Tile.Y - 1);

	TMap<FString, bool> SidesTouching;
	SidesTouching.Add(TEXT("N"), TileNoiseN <= Threshold);
	UE_LOG(LogTemp, Warning, TEXT("Checking Threshold = %f against TileNoiseN = %f"), Threshold, TileNoiseN);
	SidesTouching.Add(TEXT("NE"), TileNoiseNE <= Threshold);
	SidesTouching.Add(TEXT("E"), TileNoiseE <= Threshold);
	SidesTouching.Add(TEXT("SE"), TileNoiseSE <= Threshold);
	SidesTouching.Add(TEXT("S"), TileNoiseS <= Threshold);
	SidesTouching.Add(TEXT("SW"), TileNoiseSW <= Threshold);
	SidesTouching.Add(TEXT("W"), TileNoiseW <= Threshold);
	SidesTouching.Add(TEXT("NW"), TileNoiseNW <= Threshold);

	return SidesTouching;
}

bool AProceduralAnimatedTileMap::TileIsSuroundedByOnAllSides(FVector2D Tile, float Threshold) {

	float TileNoiseN = AProceduralAnimatedTileMap::GetNoise(Tile.X, Tile.Y - 1);
	float TileNoiseNE = AProceduralAnimatedTileMap::GetNoise(Tile.X + 1, Tile.Y - 1);
	float TileNoiseE = AProceduralAnimatedTileMap::GetNoise(Tile.X + 1, Tile.Y);
	float TileNoiseSE = AProceduralAnimatedTileMap::GetNoise(Tile.X + 1, Tile.Y + 1);
	float TileNoiseS = AProceduralAnimatedTileMap::GetNoise(Tile.X, Tile.Y + 1);
	float TileNoiseSW = AProceduralAnimatedTileMap::GetNoise(Tile.X - 1, Tile.Y + 1);
	float TileNoiseW = AProceduralAnimatedTileMap::GetNoise(Tile.X - 1, Tile.Y);
	float TileNoiseNW = AProceduralAnimatedTileMap::GetNoise(Tile.X - 1, Tile.Y - 1);

	return	(
		TileNoiseN > Threshold &&
		TileNoiseNE > Threshold &&
		TileNoiseE > Threshold &&
		TileNoiseSE > Threshold &&
		TileNoiseS > Threshold &&
		TileNoiseSW > Threshold &&
		TileNoiseW > Threshold &&
		TileNoiseNW > Threshold
	);
}

bool AProceduralAnimatedTileMap::TileIsSuroundedByOnAllSidesTwoDeep(FVector2D Tile, float Threshold) {

	float TileNoiseN = AProceduralAnimatedTileMap::GetNoise(Tile.X, Tile.Y - 2);
	float TileNoiseNxNE = AProceduralAnimatedTileMap::GetNoise(Tile.X + 1, Tile.Y - 2);
	float TileNoiseNE = AProceduralAnimatedTileMap::GetNoise(Tile.X + 2, Tile.Y - 2);
	float TileNoiseExNE = AProceduralAnimatedTileMap::GetNoise(Tile.X + 2, Tile.Y - 1);
	float TileNoiseE = AProceduralAnimatedTileMap::GetNoise(Tile.X + 2, Tile.Y);
	float TileNoiseExSE = AProceduralAnimatedTileMap::GetNoise(Tile.X + 2, Tile.Y + 1);
	float TileNoiseSE = AProceduralAnimatedTileMap::GetNoise(Tile.X + 2, Tile.Y + 2);
	float TileNoiseSxSE = AProceduralAnimatedTileMap::GetNoise(Tile.X + 1, Tile.Y + 2);
	float TileNoiseS = AProceduralAnimatedTileMap::GetNoise(Tile.X, Tile.Y + 2);
	float TileNoiseSxSW = AProceduralAnimatedTileMap::GetNoise(Tile.X - 1, Tile.Y + 2);
	float TileNoiseSW = AProceduralAnimatedTileMap::GetNoise(Tile.X - 2, Tile.Y + 2);
	float TileNoiseWxSW = AProceduralAnimatedTileMap::GetNoise(Tile.X + 2, Tile.Y + 1);
	float TileNoiseW = AProceduralAnimatedTileMap::GetNoise(Tile.X - 2, Tile.Y);
	float TileNoiseWxNW = AProceduralAnimatedTileMap::GetNoise(Tile.X + 2, Tile.Y - 1);
	float TileNoiseNW = AProceduralAnimatedTileMap::GetNoise(Tile.X - 2, Tile.Y - 2);
	float TileNoiseNxNW = AProceduralAnimatedTileMap::GetNoise(Tile.X - 1, Tile.Y - 2);

	return	(
		TileNoiseN > Threshold &&
		TileNoiseNxNE > Threshold &&
		TileNoiseNE > Threshold &&
		TileNoiseExNE > Threshold &&
		TileNoiseE > Threshold &&
		TileNoiseExSE > Threshold &&
		TileNoiseSE > Threshold &&
		TileNoiseSxSE > Threshold &&
		TileNoiseS > Threshold &&
		TileNoiseSxSW > Threshold &&
		TileNoiseSW > Threshold &&
		TileNoiseSxSW > Threshold &&
		TileNoiseW > Threshold &&
		TileNoiseWxNW > Threshold &&
		TileNoiseNW > Threshold &&
		TileNoiseNxNW > Threshold
	);
}


float AProceduralAnimatedTileMap::GetNoise(FVector2D Tile) {
	float noise;
	for (auto& Elem : Noise)
	{
		if (Elem.Key.Equals(Tile)) {
			noise = Elem.Value;
		}
	}

	return noise;
}

float AProceduralAnimatedTileMap::GetNoise(float X, float Y) {
	return AProceduralAnimatedTileMap::GetNoise(FVector2D(X,Y));
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