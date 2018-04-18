// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipPawn.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PaperSpriteComponent.h"
#include "Components/InputComponent.h"
#include "RiggingActor.h"

FName AShipPawn::HullSpriteComponentName(TEXT("HullSprite"));
FName AShipPawn::RiggingActorName(TEXT("RiggingActor"));

// Sets default values
AShipPawn::AShipPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UArrowComponent* arrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));

	if (!RootComponent) {
		RootComponent = ShipForward = arrowComponent;
	} else {
		ShipForward = arrowComponent;
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

	USpringArmComponent* SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetArmLength = 500.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = false;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->CameraLagSpeed = 2.0f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetWorldRotation(FRotator(-90.0f, 0.0f, 0.0f));

	ShipCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ShipCamera"));
	ShipCamera->bUsePawnControlRotation = false;
	ShipCamera->ProjectionMode = ECameraProjectionMode::Orthographic;
	ShipCamera->OrthoWidth = 1024.0f;
	ShipCamera->AspectRatio = 3.0f / 4.0f;
	ShipCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	ShipCamera->SetWorldRotation(FRotator(-90.0f, 0.0f, 0.0f));

	ShipRigging = CreateOptionalDefaultSubobject<UChildActorComponent>(AShipPawn::RiggingActorName);
	ShipRigging->SetupAttachment(HullSprite);

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

	InputComponent->BindAxis("Rotate", this, &AShipPawn::Rotate);
	InputComponent->BindAxis("MoveForward", this, &AShipPawn::MoveForward);

}

void AShipPawn::Rotate(float AxisValue) {



}

void AShipPawn::MoveForward(float AxisValue) {

}

