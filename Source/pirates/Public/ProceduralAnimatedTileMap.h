// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimatedTileMap.h"
#include "Fastnoise/FastNoise.h"
#include "ProceduralAnimatedTileMap.generated.h"

/**
 * 
 */
UCLASS()
class PIRATES_API AProceduralAnimatedTileMap : public AAnimatedTileMap
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AProceduralAnimatedTileMap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GenerateMap();

protected:

	/** The base tile set used in map generation. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation", meta = (AllowPrivateAccess = "true"))
		class UPaperTileSet* BaseTileSet;

	/** The number of rows the generated map should have. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation", meta = (AllowPrivateAccess = "true"))
		int32 Rows = 10;

	/** The number of columns the generated map should have. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation", meta = (AllowPrivateAccess = "true"))
		int32 Columns = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation", meta = (AllowPrivateAccess = "true"))
		float WaterLevel = 100.0f;

	/** The same seed will always generate the same map with the same settings. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation", meta = (AllowPrivateAccess = "true"))
		int32 Seed = FMath::RandRange(0, 999999);

	/** The type of noise used to generate the map. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation|Noise", meta = (AllowPrivateAccess = "true"))
		ENoiseType NoiseType = ENoiseType::Simplex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation|Noise", meta = (AllowPrivateAccess = "true"))
		float Frequency = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation|Noise", meta = (AllowPrivateAccess = "true"))
		EInterp Interpolation = EInterp::InterpLinear;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation|Fractal", meta = (AllowPrivateAccess = "true"))
		EFractalType FractalType = EFractalType::FBM;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation|Fractal", meta = (AllowPrivateAccess = "true"))
		float Octaves = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation|Fractal", meta = (AllowPrivateAccess = "true"))
		float Lacunarity = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation|Fractal", meta = (AllowPrivateAccess = "true"))
		float Gain = 0.4f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation|Cellular", meta = (AllowPrivateAccess = "true"))
		ECellularDistanceFunction CellularDistance = ECellularDistanceFunction::Natural;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation|Cellular", meta = (AllowPrivateAccess = "true"))
		ECellularReturnType CellularReturnType = ECellularReturnType::Distance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation|Warp", meta = (AllowPrivateAccess = "true"))
		EPositionWarpType WarpType = EPositionWarpType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation|Warp", meta = (AllowPrivateAccess = "true"))
		float WarpAmp = 1.0f;


	UPROPERTY(Transient, meta = (AllowPrivateAccess = "true"))
		class UFastNoise * FastNoise;

	UPROPERTY(Transient, meta = (AllowPrivateAccess = "true"))
		class UFastNoise * CellNoise;

	class TMultiMap<FString, int32> TileTypes;
	
	void PositionMapTopDown();

	void PlaceTile(int32 TileX, int32 TileY, int32 LayerIndex, FString TileType);
	
};
