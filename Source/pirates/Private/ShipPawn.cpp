// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipPawn.h"
#include "PaperFlipbookComponent.h"
#include "Components/ArrowComponent.h"

FName AShipPawn::HullSpriteComponentName(TEXT("HullSprite"));
FName AShipPawn::RiggingSpriteComponentName(TEXT("RiggingSprite"));

// Sets default values
AShipPawn::AShipPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent) {
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ShipRoot"));
	}

	ShipForward = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ShipForward->SetupAttachment(RootComponent);

	// Try to create the hull sprite component
	HullSprite = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(AShipPawn::HullSpriteComponentName);
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

	// Try to create the rigging sprite component
	RiggingSprite = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(AShipPawn::RiggingSpriteComponentName);
	if (RiggingSprite)
	{
		//HullSprite->AlwaysLoadOnClient = true;
		//HullSprite->AlwaysLoadOnServer = true;
		//HullSprite->bOwnerNoSee = false;
		//HullSprite->bAffectDynamicIndirectLighting = true;
		//HullSprite->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		RiggingSprite->SetupAttachment(HullSprite);
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


