// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperFlipbook.h"
#include "AnimatedTileMap.generated.h"


UCLASS()
class PIRATES_API AAnimatedTileMap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAnimatedTileMap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** The base tile map associated with the (optional sub-object). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Map", meta = (AllowPrivateAccess = "true", MetaClass = "UPaperTileMap"))
		class UPaperTileMapComponent* BaseTileMap;

	/** Add entries in BP Defaults, or during Runtime! Iterate over them using the For Each Loop BP Node */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map|Animated Tiles", meta = (AllowPrivateAccess = "true", MetaClass = "UPaperFlipbook"))
		TArray<UPaperFlipbook*> AnimatedTiles;

	/** Component shown in the editor only to indicate north on plane facing */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* NorthArrow;

	void SetUpMapAnimation();

	class TMultiMap<FString, int32> ExtractAllTileUserData(FString MetadataLabel);

	class FString ExtractTileUserData(int32 TileX, int32 TileY, int32 LayerIndex, FString MetadataLabel);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
