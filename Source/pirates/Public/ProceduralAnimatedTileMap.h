// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimatedTileMap.h"
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

private:

	/** The base tile set used in map generation. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation", meta = (AllowPrivateAccess = "true"))
		class UPaperTileSet* BaseTileSet;

	/** The number of rows the generated map should have. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation", meta = (AllowPrivateAccess = "true"))
		int32 Rows = 4;

	/** The number of columns the generated map should have. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation", meta = (AllowPrivateAccess = "true"))
		int32 Columns = 4;

	/** The number of columns the generated map should have. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation", meta = (AllowPrivateAccess = "true"))
		int32 Seed = FMath::RandRange(0, 999999);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation", meta = (AllowPrivateAccess = "true"))
		float Frequency = 0.95f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation", meta = (AllowPrivateAccess = "true"))
		float Octaves = 0.95f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation", meta = (AllowPrivateAccess = "true"))
		float Gain = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation", meta = (AllowPrivateAccess = "true"))
		float Lacunarity = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation", meta = (AllowPrivateAccess = "true"))
		float WaterLevel = 0.85f;

	UPROPERTY()
		class UFastNoise * FastNoise;

	class TMultiMap<FString, int32> TileTypes;
	
	void PositionMapTopDown();

	void PlaceTile(int32 TileX, int32 TileY, int32 LayerIndex, FString TileType);
	
};
