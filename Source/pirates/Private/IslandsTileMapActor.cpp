// Fill out your copyright notice in the Description page of Project Settings.

#include "IslandsTileMapActor.h"

#include "PaperTileMapComponent.h"

// Sets default values
AIslandsTileMapActor::AIslandsTileMapActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	if (RootComponent != NULL)
	{
		RootComponent->DestroyComponent();
	}

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	BaseTileMap = CreateDefaultSubobject<UPaperTileMapComponent>(TEXT("BaseTileMap"));
	BaseTileMap->SetupAttachment(RootComponent);
}

// Called every frame
void AIslandsTileMapActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AIslandsTileMapActor::PostInitProperties()
{
	Super::PostInitProperties();
}

#if WITH_EDITOR
void AIslandsTileMapActor::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

// Called when the game starts or when spawned
void AIslandsTileMapActor::BeginPlay()
{
	Super::BeginPlay();
}
