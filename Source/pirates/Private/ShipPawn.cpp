// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipPawn.h"
#include "Components/ArrowComponent.h"
#include "PaperSpriteComponent.h"

FName AShipPawn::HullSpriteComponentName(TEXT("HullSprite"));

// Sets default values
AShipPawn::AShipPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent) {
		RootComponent = ShipForward = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	} else {
		ShipForward = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	}

	// Try to create the hull sprite component
	HullSprite = CreateOptionalDefaultSubobject<UPaperSpriteComponent>(AShipPawn::HullSpriteComponentName);
	if (HullSprite)
	{
		//HullSprite->AlwaysLoadOnClient = true;
		//HullSprite->AlwaysLoadOnServer = true;
		//HullSprite->bOwnerNoSee = false;
		//HullSprite->bAffectDynamicIndirectLighting = true;
		//HullSprite->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		HullSprite->SetupAttachment(ShipForward);
		//static FName CollisionProfileName(TEXT("CharacterMesh"));
		//HullSprite->SetCollisionProfileName(CollisionProfileName);
		//HullSprite->bGenerateOverlapEvents = false;
	}

}

// Called when the game starts or when spawned
void AShipPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShipPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AShipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


