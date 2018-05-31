// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "PaperTileMap.h"
#include "PaperTileSet.h"
#include "PaperTileMapComponent.h"
#include "PaperTileLayer.h"
#include "PiratesMap.h"
#include "UObject/ObjectMacros.h"
#include "GameFramework/Actor.h"

#include "IslandsTileMapActor.generated.h"

UCLASS()
class PIRATES_API AIslandsTileMapActor : public AActor, public IPiratesMap
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AIslandsTileMapActor();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
#endif

	/** Impl for FindTileInfoAtWorldLoation from IPiratesMap */
	UFUNCTION(BlueprintNativeEvent, BlueprintNativeEvent, Category = "PiratesMap")
	TArray<FPaperTileInfo> FindTileInfoAtWorldLoation(FVector WorldLocation);
	virtual TArray<FPaperTileInfo> FindTileInfoAtWorldLoation_Implementation(FVector WorldLocation) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Map", meta = (AllowPrivateAccess = "true"))
	class UPaperTileMapComponent *TileMapComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostRegisterAllComponents() override;

	FORCEINLINE class UPaperTileMapComponent *GetTileMapComponent() const { return TileMapComponent; }
};
