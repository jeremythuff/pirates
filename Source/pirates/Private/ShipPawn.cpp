// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipPawn.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "ShipFloatingPawnMovement.h"
#include "RiggingActor.h"

FName AShipPawn::HullSpriteComponentName(TEXT("HullSprite"));
FName AShipPawn::RiggingActorName(TEXT("RiggingActor"));

// Sets default values
AShipPawn::AShipPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent) {
		//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ShipBase"));
		RootComponent = ShipCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ShipBase"));
	}
	
	ShipForward = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ShipForward->SetupAttachment(RootComponent);

	// Try to create the hull sprite component
	HullSprite = CreateOptionalDefaultSubobject<UPaperSpriteComponent>(AShipPawn::HullSpriteComponentName);
	if (HullSprite)
	{
		/*HullSprite->AlwaysLoadOnClient = true;
		HullSprite->AlwaysLoadOnServer = true;
		HullSprite->bOwnerNoSee = false;
		HullSprite->bAffectDynamicIndirectLighting = true;
		HullSprite->PrimaryComponentTick.TickGroup = TG_PrePhysics;*/
		HullSprite->SetupAttachment(ShipForward);
		/*static FName CollisionProfileName(TEXT("CharacterMesh"));
		HullSprite->SetCollisionProfileName(CollisionProfileName);
		HullSprite->bGenerateOverlapEvents = false;*/
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetArmLength = 1000.0f;
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
	ShipCamera->OrthoWidth = 2048.0f;
	ShipCamera->AspectRatio = 2.39f / 1.0f;
	ShipCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	ShipCamera->SetWorldRotation(FRotator(-90.0f, 0.0f, 0.0f));

	ShipRigging = CreateOptionalDefaultSubobject<UChildActorComponent>(AShipPawn::RiggingActorName);
	ShipRigging->SetupAttachment(HullSprite);

	ShipRipple = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(TEXT("ShipRipple"));
	ShipRipple->SetupAttachment(HullSprite);

	// Create an instance of our movement component, and tell it to update the root.
	ShipMovementComponent = CreateDefaultSubobject<UShipFloatingPawnMovement>(TEXT("ShipFloatingPawnMovement"));

}

UPawnMovementComponent* AShipPawn::GetMovementComponent() const
{
	return ShipMovementComponent;
}

// Called when the game starts or when spawned
void AShipPawn::BeginPlay()
{
	Super::BeginPlay();
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;
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

	InputComponent->BindAxis("MoveForward", this, &AShipPawn::MoveForward);
	InputComponent->BindAxis("Turn", this, &AShipPawn::Turn);

}

void AShipPawn::MoveForward(float AxisValue)
{
	if (ShipMovementComponent && (ShipMovementComponent->UpdatedComponent == RootComponent))
	{
		ShipMovementComponent->AddInputVector(GetActorForwardVector() * (FMath::Abs(AxisValue)));
	}
}

void AShipPawn::Turn(float AxisValue)
{
	float halfAxisValue = AxisValue / 2;
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += (AxisValue);
	SetActorRotation(NewRotation);
	AShipPawn::MoveForward(FMath::Abs(halfAxisValue));
}
