// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "PaperTileMapComponent.h"
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

  virtual void Tick(float DeltaTime) override;

  virtual void PreInitializeComponents() override;

  virtual void PostInitializeComponents() override;

  virtual void PostRegisterAllComponents() override;

  /** Impl for FindTileInfoAtWorldLoation from IPiratesMap */
  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BaseMap")
  TArray<FPaperTileInfo> FindTileInfoAtWorldLoation(FVector WorldLocation);
  virtual TArray<FPaperTileInfo> FindTileInfoAtWorldLoation_Implementation(FVector WorldLocation) override;

protected:
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseMap", meta = (AllowPrivateAccess = "true"))
  class UPaperTileMapComponent *TileMapComponent;

  // Called when the game starts or when spawned
  virtual void BeginPlay() override;
};
