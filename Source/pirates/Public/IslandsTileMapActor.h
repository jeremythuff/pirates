// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperTileMapComponent.h"
#include "UObject/ObjectMacros.h"
#include "GameFramework/Actor.h"

#include "IslandsTileMapActor.generated.h"

UCLASS()
class PIRATES_API AIslandsTileMapActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AIslandsTileMapActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitProperties() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
#endif

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Map", meta = (AllowPrivateAccess = "true"))
	class UPaperTileMapComponent *BaseTileMap;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
