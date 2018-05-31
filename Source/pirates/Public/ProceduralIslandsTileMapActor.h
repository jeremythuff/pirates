// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>

#include "CoreMinimal.h"
#include "Perlin.h"
#include "IslandsTileMapActor.h"

#include "ProceduralIslandsTileMapActor.generated.h"

typedef std::vector<float> Vector;
typedef std::vector<Vector> Matrix;

DECLARE_LOG_CATEGORY_EXTERN(ProceduralLog, Log, All);

UCLASS()
class PIRATES_API AProceduralIslandsTileMapActor : public AIslandsTileMapActor
{
	GENERATED_BODY()

public:
	// Sets Default values for this actor"s properties
	AProceduralIslandsTileMapActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostRegisterAllComponents() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
#endif

	void Init();

	UFUNCTION(BlueprintCallable, Category = "PiratesMap|Procedural Generation", meta = (DefaultToSelf = "Controller"))
	void Generate();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation", meta = (AllowPrivateAccess = "true"))
	class UPaperTileSet *TileSet;

	/** The number of rows the generated map should have. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation", meta = (AllowPrivateAccess = "true"))
	int32 Rows = 128;

	/** The number of columns the generated map should have. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation", meta = (AllowPrivateAccess = "true"))
	int32 Columns = 128;

	/** The random seed for perlin noise. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation", meta = (AllowPrivateAccess = "true"))
	int Seed = 1;

	/** The frequency for perlin noise. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation", meta = (AllowPrivateAccess = "true"))
	float Frequency = 12.0f;

	/** The octaves for perlin noise. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation", meta = (AllowPrivateAccess = "true"))
	int Octaves = 1;

	/** Whether to use absolute perlin result. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation", meta = (AllowPrivateAccess = "true", ClampMin = "0", ClampMax = "100"))
	int Density = 100;

	/** The sea level. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation", meta = (AllowPrivateAccess = "true"))
	float SeaLevel = 100.0f;

	/** The water tile index. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation", meta = (AllowPrivateAccess = "true"))
	int32 WaterTileIndex = 104;

	/** The likelyhood of a shallow rock. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation", meta = (AllowPrivateAccess = "true", ClampMin = "0", ClampMax = "1"))
	float ShallowsRockLikelihood = 0.025f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation", meta = (AllowPrivateAccess = "true"))
	TArray<int32> ShallowsRockTileIndeces = InitialShallowsRockTileIndeces();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation", meta = (AllowPrivateAccess = "true"))
	TArray<int32> GrassTileIndeces = InitialGrassTileIndices();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation", meta = (AllowPrivateAccess = "true"))
	TArray<int32> FoilageTileIndeces = InitialFoliageTileIndices();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation", meta = (AllowPrivateAccess = "true"))
	TMap<FString, FString> ShallowsEdgeMap = InitialShallowsEdgeMap();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation", meta = (AllowPrivateAccess = "true"))
	TMap<FString, FString> LandEdgeMap = InitialLandEdgeMap();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Map|Procedural Generation", meta = (AllowPrivateAccess = "true"))
	TMap<FString, FString> Edges = InitialEdges();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	float Scale = 1000.0f;

	float ShallowsLevel;

	void Resize();

	void Reposition();

	TMap<int32, UPaperTileLayer> IslandLayers;

	Matrix GenerateNoise();

	Matrix TrimNoise(Matrix Noise, float Level);

	FString GetSurroundings(Matrix Noise, int32 X, int32 Y, float Level);

	Matrix CopyNoise(Matrix Noise);

	void AddTile(int32 C, int32 Y, int32 LayerIndex, int32 TileIndex);

	FORCEINLINE TArray<int32> InitialShallowsRockTileIndeces()
	{
		TArray<int32> InitialShallowsRockTileIndeces;
		InitialShallowsRockTileIndeces.Add(48);
		InitialShallowsRockTileIndeces.Add(49);
		InitialShallowsRockTileIndeces.Add(50);
		InitialShallowsRockTileIndeces.Add(64);
		InitialShallowsRockTileIndeces.Add(65);
		InitialShallowsRockTileIndeces.Add(66);
		return InitialShallowsRockTileIndeces;
	}

	FORCEINLINE TArray<int32> InitialGrassTileIndices()
	{
		TArray<int32> InitialGrassTileIndices;
		InitialGrassTileIndices.Add(22);
		InitialGrassTileIndices.Add(22);
		InitialGrassTileIndices.Add(22);
		InitialGrassTileIndices.Add(22);
		InitialGrassTileIndices.Add(23);
		InitialGrassTileIndices.Add(23);
		InitialGrassTileIndices.Add(23);
		InitialGrassTileIndices.Add(23);
		InitialGrassTileIndices.Add(23);
		InitialGrassTileIndices.Add(23);
		InitialGrassTileIndices.Add(38);
		InitialGrassTileIndices.Add(39);
		return InitialGrassTileIndices;
	}

	FORCEINLINE TArray<int32> InitialFoliageTileIndices()
	{
		TArray<int32> IntialFoliageTileIndices;
		IntialFoliageTileIndices.Add(69);
		IntialFoliageTileIndices.Add(69);
		IntialFoliageTileIndices.Add(69);
		IntialFoliageTileIndices.Add(69);
		IntialFoliageTileIndices.Add(69);
		IntialFoliageTileIndices.Add(70);
		IntialFoliageTileIndices.Add(70);
		IntialFoliageTileIndices.Add(70);
		IntialFoliageTileIndices.Add(70);
		IntialFoliageTileIndices.Add(70);
		IntialFoliageTileIndices.Add(71);
		IntialFoliageTileIndices.Add(86);
		IntialFoliageTileIndices.Add(87);
		return IntialFoliageTileIndices;
	}

	FORCEINLINE TMap<FString, FString> InitialEdges()
	{
		TMap<FString, FString> InitialEdges;
		InitialEdges.Add("00000000", "Default");
		InitialEdges.Add("00000001", "Right");
		InitialEdges.Add("00000010", "InnerBottomRight");
		InitialEdges.Add("00000011", "Right");
		InitialEdges.Add("00000100", "Bottom");
		InitialEdges.Add("00000101", "BottomRight");
		InitialEdges.Add("00000110", "Bottom");
		InitialEdges.Add("00000111", "BottomRight");
		InitialEdges.Add("00001000", "InnerBottomLeft");
		InitialEdges.Add("00001001", "BottomRight");
		InitialEdges.Add("00001010", "Unknown");
		InitialEdges.Add("00001011", "Unknown");
		InitialEdges.Add("00001100", "Bottom");
		InitialEdges.Add("00001101", "BottomRight");
		InitialEdges.Add("00001110", "Bottom");
		InitialEdges.Add("00001111", "BottomRight");
		InitialEdges.Add("00010000", "Left");
		InitialEdges.Add("00010001", "Unknown");
		InitialEdges.Add("00010010", "BottomLeft");
		InitialEdges.Add("00010011", "Unknown");
		InitialEdges.Add("00010100", "BottomLeft");
		InitialEdges.Add("00010101", "Unknown");
		InitialEdges.Add("00010110", "BottomLeft");
		InitialEdges.Add("00010111", "Unknown");
		InitialEdges.Add("00011000", "Left");
		InitialEdges.Add("00011001", "Unknown");
		InitialEdges.Add("00011010", "Unknown");
		InitialEdges.Add("00011011", "Unknown");
		InitialEdges.Add("00011100", "BottomLeft");
		InitialEdges.Add("00011101", "Unknown");
		InitialEdges.Add("00011110", "BottomLeft");
		InitialEdges.Add("00011111", "Unknown");
		InitialEdges.Add("00100000", "InnerTopLeft");
		InitialEdges.Add("00100001", "TopRight");
		InitialEdges.Add("00100010", "Unknown");
		InitialEdges.Add("00100011", "Unknown");
		InitialEdges.Add("00100100", "BottomLeft");
		InitialEdges.Add("00100101", "Unknown");
		InitialEdges.Add("00100110", "Unknown");
		InitialEdges.Add("00100111", "Unknown");
		InitialEdges.Add("00101000", "Unknown");
		InitialEdges.Add("00101001", "Unknown");
		InitialEdges.Add("00101010", "Unknown");
		InitialEdges.Add("00101011", "Unknown");
		InitialEdges.Add("00101100", "Unknown");
		InitialEdges.Add("00101101", "Unknown");
		InitialEdges.Add("00101110", "Unknown");
		InitialEdges.Add("00101111", "Unknown");
		InitialEdges.Add("00110000", "Left");
		InitialEdges.Add("00110001", "Unknown");
		InitialEdges.Add("00110010", "Unknown");
		InitialEdges.Add("00110011", "Unknown");
		InitialEdges.Add("00110100", "BottomLeft");
		InitialEdges.Add("00110101", "Unknown");
		InitialEdges.Add("00110110", "BottomLeft");
		InitialEdges.Add("00110111", "Unknown");
		InitialEdges.Add("00111000", "Left");
		InitialEdges.Add("00111001", "Unknown");
		InitialEdges.Add("00111010", "Unknown");
		InitialEdges.Add("00111011", "Unknown");
		InitialEdges.Add("00111100", "BottomLeft");
		InitialEdges.Add("00111101", "Unknown");
		InitialEdges.Add("00111110", "BottomLeft");
		InitialEdges.Add("00111111", "Unknown");
		InitialEdges.Add("01000000", "Top");
		InitialEdges.Add("01000001", "TopRight");
		InitialEdges.Add("01000010", "TopRight");
		InitialEdges.Add("01000011", "TopRight");
		InitialEdges.Add("01000100", "Unknown");
		InitialEdges.Add("01000101", "Unknown");
		InitialEdges.Add("01000110", "Unknown");
		InitialEdges.Add("01000111", "Unknown");
		InitialEdges.Add("01001000", "TopLeft");
		InitialEdges.Add("01001001", "Unknown");
		InitialEdges.Add("01001010", "Unknown");
		InitialEdges.Add("01001011", "Unknown");
		InitialEdges.Add("01001100", "Unknown");
		InitialEdges.Add("01001101", "Unknown");
		InitialEdges.Add("01001110", "Unknown");
		InitialEdges.Add("01001111", "Unknown");
		InitialEdges.Add("01010000", "TopLeft");
		InitialEdges.Add("01010001", "Unknown");
		InitialEdges.Add("01010010", "Unknown");
		InitialEdges.Add("01010011", "Unknown");
		InitialEdges.Add("01010100", "Unknown");
		InitialEdges.Add("01010101", "Unknown");
		InitialEdges.Add("01010110", "Unknown");
		InitialEdges.Add("01010111", "Unknown");
		InitialEdges.Add("01011000", "TopLeft");
		InitialEdges.Add("01011001", "Unknown");
		InitialEdges.Add("01011010", "Unknown");
		InitialEdges.Add("01011011", "Unknown");
		InitialEdges.Add("01011100", "Unknown");
		InitialEdges.Add("01011101", "Unknown");
		InitialEdges.Add("01011110", "Unknown");
		InitialEdges.Add("01011111", "Unknown");
		InitialEdges.Add("01100000", "Top");
		InitialEdges.Add("01100001", "TopRight");
		InitialEdges.Add("01100010", "Unknown");
		InitialEdges.Add("01100011", "Unknown");
		InitialEdges.Add("01100100", "Unknown");
		InitialEdges.Add("01100101", "Unknown");
		InitialEdges.Add("01100110", "Unknown");
		InitialEdges.Add("01100111", "Unknown");
		InitialEdges.Add("01101000", "Unknown");
		InitialEdges.Add("01101001", "Unknown");
		InitialEdges.Add("01101010", "Unknown");
		InitialEdges.Add("01101011", "Unknown");
		InitialEdges.Add("01101100", "Unknown");
		InitialEdges.Add("01101101", "Unknown");
		InitialEdges.Add("01101110", "Unknown");
		InitialEdges.Add("01101111", "Unknown");
		InitialEdges.Add("01110000", "TopLeft");
		InitialEdges.Add("01110001", "Unknown");
		InitialEdges.Add("01110010", "Unknown");
		InitialEdges.Add("01110011", "Unknown");
		InitialEdges.Add("01110100", "Unknown");
		InitialEdges.Add("01110101", "Unknown");
		InitialEdges.Add("01110110", "Unknown");
		InitialEdges.Add("01110111", "Unknown");
		InitialEdges.Add("01111000", "TopLeft");
		InitialEdges.Add("01111001", "Unknown");
		InitialEdges.Add("01111010", "Unknown");
		InitialEdges.Add("01111011", "Unknown");
		InitialEdges.Add("01111100", "Unknown");
		InitialEdges.Add("01111101", "Unknown");
		InitialEdges.Add("01111110", "Unknown");
		InitialEdges.Add("01111111", "Unknown");
		InitialEdges.Add("10000000", "InnerTopRight");
		InitialEdges.Add("10000001", "Right");
		InitialEdges.Add("10000010", "Unknown");
		InitialEdges.Add("10000011", "Right");
		InitialEdges.Add("10000100", "BottomRight");
		InitialEdges.Add("10000101", "BottomRight");
		InitialEdges.Add("10000110", "Unknown");
		InitialEdges.Add("10000111", "BottomRight");
		InitialEdges.Add("10001000", "Unknown");
		InitialEdges.Add("10001001", "BottomRight");
		InitialEdges.Add("10001010", "Unknown");
		InitialEdges.Add("10001011", "Unknown");
		InitialEdges.Add("10001100", "Unknown");
		InitialEdges.Add("10001101", "Unknown");
		InitialEdges.Add("10001110", "Unknown");
		InitialEdges.Add("10001111", "BottomRight");
		InitialEdges.Add("10010000", "TopLeft");
		InitialEdges.Add("10010001", "Unknown");
		InitialEdges.Add("10010010", "Unknown");
		InitialEdges.Add("10010011", "Unknown");
		InitialEdges.Add("10010100", "Unknown");
		InitialEdges.Add("10010101", "Unknown");
		InitialEdges.Add("10010110", "Unknown");
		InitialEdges.Add("10010111", "Unknown");
		InitialEdges.Add("10011000", "TopLeft");
		InitialEdges.Add("10011001", "Unknown");
		InitialEdges.Add("10011010", "Unknown");
		InitialEdges.Add("10011011", "Unknown");
		InitialEdges.Add("10011100", "Unknown");
		InitialEdges.Add("10011101", "Unknown");
		InitialEdges.Add("10011110", "Unknown");
		InitialEdges.Add("10011111", "Unknown");
		InitialEdges.Add("10100000", "Unknown");
		InitialEdges.Add("10100001", "Unknown");
		InitialEdges.Add("10100010", "Unknown");
		InitialEdges.Add("10100011", "Unknown");
		InitialEdges.Add("10100100", "Unknown");
		InitialEdges.Add("10100101", "Unknown");
		InitialEdges.Add("10100110", "Unknown");
		InitialEdges.Add("10100111", "Unknown");
		InitialEdges.Add("10101000", "Unknown");
		InitialEdges.Add("10101001", "Unknown");
		InitialEdges.Add("10101010", "Unknown");
		InitialEdges.Add("10101011", "Unknown");
		InitialEdges.Add("10101100", "Unknown");
		InitialEdges.Add("10101101", "Unknown");
		InitialEdges.Add("10101110", "Unknown");
		InitialEdges.Add("10101111", "Unknown");
		InitialEdges.Add("10110000", "Unknown");
		InitialEdges.Add("10110001", "Unknown");
		InitialEdges.Add("10110010", "Unknown");
		InitialEdges.Add("10110011", "Unknown");
		InitialEdges.Add("10110100", "Unknown");
		InitialEdges.Add("10110101", "Unknown");
		InitialEdges.Add("10110110", "Unknown");
		InitialEdges.Add("10110111", "Unknown");
		InitialEdges.Add("10111000", "Unknown");
		InitialEdges.Add("10111001", "Unknown");
		InitialEdges.Add("10111010", "Unknown");
		InitialEdges.Add("10111011", "Unknown");
		InitialEdges.Add("10111100", "Unknown");
		InitialEdges.Add("10111101", "Unknown");
		InitialEdges.Add("10111110", "Unknown");
		InitialEdges.Add("10111111", "Unknown");
		InitialEdges.Add("11000000", "Top");
		InitialEdges.Add("11000001", "TopRight");
		InitialEdges.Add("11000010", "Unknown");
		InitialEdges.Add("11000011", "TopRight");
		InitialEdges.Add("11000100", "Unknown");
		InitialEdges.Add("11000101", "Unknown");
		InitialEdges.Add("11000110", "Unknown");
		InitialEdges.Add("11000111", "Unknown");
		InitialEdges.Add("11001000", "Unknown");
		InitialEdges.Add("11001001", "Unknown");
		InitialEdges.Add("11001010", "Unknown");
		InitialEdges.Add("11001011", "Unknown");
		InitialEdges.Add("11001100", "Unknown");
		InitialEdges.Add("11001101", "Unknown");
		InitialEdges.Add("11001110", "Unknown");
		InitialEdges.Add("11001111", "Unknown");
		InitialEdges.Add("11010000", "TopLeft");
		InitialEdges.Add("11010001", "Unknown");
		InitialEdges.Add("11010010", "Unknown");
		InitialEdges.Add("11010011", "Unknown");
		InitialEdges.Add("11010100", "Unknown");
		InitialEdges.Add("11010101", "Unknown");
		InitialEdges.Add("11010110", "Unknown");
		InitialEdges.Add("11010111", "Unknown");
		InitialEdges.Add("11011000", "Unknown");
		InitialEdges.Add("11011001", "Unknown");
		InitialEdges.Add("11011010", "Unknown");
		InitialEdges.Add("11011011", "Unknown");
		InitialEdges.Add("11011100", "Unknown");
		InitialEdges.Add("11011101", "Unknown");
		InitialEdges.Add("11011110", "Unknown");
		InitialEdges.Add("11011111", "Unknown");
		InitialEdges.Add("11100000", "Top");
		InitialEdges.Add("11100001", "TopRight");
		InitialEdges.Add("11100010", "Unknown");
		InitialEdges.Add("11100011", "TopRight");
		InitialEdges.Add("11100100", "Unknown");
		InitialEdges.Add("11100101", "Unknown");
		InitialEdges.Add("11100110", "Unknown");
		InitialEdges.Add("11100111", "Unknown");
		InitialEdges.Add("11101000", "Unknown");
		InitialEdges.Add("11101001", "Unknown");
		InitialEdges.Add("11101010", "Unknown");
		InitialEdges.Add("11101011", "Unknown");
		InitialEdges.Add("11101100", "Unknown");
		InitialEdges.Add("11101101", "Unknown");
		InitialEdges.Add("11101110", "Unknown");
		InitialEdges.Add("11101111", "Unknown");
		InitialEdges.Add("11110000", "TopLeft");
		InitialEdges.Add("11110001", "Unknown");
		InitialEdges.Add("11110010", "Unknown");
		InitialEdges.Add("11110011", "Unknown");
		InitialEdges.Add("11110100", "Unknown");
		InitialEdges.Add("11110101", "Unknown");
		InitialEdges.Add("11110110", "Unknown");
		InitialEdges.Add("11110111", "Unknown");
		InitialEdges.Add("11111000", "TopLeft");
		InitialEdges.Add("11111001", "Unknown");
		InitialEdges.Add("11111010", "Unknown");
		InitialEdges.Add("11111011", "Unknown");
		InitialEdges.Add("11111100", "Unknown");
		InitialEdges.Add("11111101", "Unknown");
		InitialEdges.Add("11111110", "Unknown");
		InitialEdges.Add("11111111", "Unknown");
		return InitialEdges;
	}

	FORCEINLINE TMap<FString, FString> InitialShallowsEdgeMap()
	{
		TMap<FString, FString> InitialShallowsEdgeMap;
		InitialShallowsEdgeMap.Add("TopRight", "9");
		InitialShallowsEdgeMap.Add("Right", "10");
		InitialShallowsEdgeMap.Add("BottomRight", "11");
		InitialShallowsEdgeMap.Add("Bottom", "27");
		InitialShallowsEdgeMap.Add("BottomLeft", "43");
		InitialShallowsEdgeMap.Add("Left", "42");
		InitialShallowsEdgeMap.Add("TopLeft", "41");
		InitialShallowsEdgeMap.Add("Top", "25");
		InitialShallowsEdgeMap.Add("InnerBottomLeft", "57");
		InitialShallowsEdgeMap.Add("InnerTopLeft", "58");
		InitialShallowsEdgeMap.Add("InnerTopRight", "74");
		InitialShallowsEdgeMap.Add("InnerBottomRight", "73");
		InitialShallowsEdgeMap.Add("Default", "26");
		InitialShallowsEdgeMap.Add("Unknown", "50");
		return InitialShallowsEdgeMap;
	}

	FORCEINLINE TMap<FString, FString> InitialLandEdgeMap()
	{
		TMap<FString, FString> InitialLandEdgeMap;
		InitialLandEdgeMap.Add("TopRight", "5");
		InitialLandEdgeMap.Add("Right", "6");
		InitialLandEdgeMap.Add("BottomRight", "8");
		InitialLandEdgeMap.Add("Bottom", "24");
		InitialLandEdgeMap.Add("BottomLeft", "56");
		InitialLandEdgeMap.Add("Left", "54");
		InitialLandEdgeMap.Add("TopLeft", "53");
		InitialLandEdgeMap.Add("Top", "21");
		InitialLandEdgeMap.Add("InnerBottomLeft", "35");
		InitialLandEdgeMap.Add("InnerTopLeft", "36");
		InitialLandEdgeMap.Add("InnerTopRight", "52");
		InitialLandEdgeMap.Add("InnerBottomRight", "51");
		InitialLandEdgeMap.Add("Default", "22");
		InitialLandEdgeMap.Add("Unknown", "66");
		return InitialLandEdgeMap;
	}
};
