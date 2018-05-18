// Fill out your copyright notice in the Description page of Project Settings.

#include "CommanderPawn.h"
#include "Components/DrawSphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "CommanderPawnMovement.h"
#include "Components/InputComponent.h"


// Sets default values
ACommanderPawn::ACommanderPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CommanderSphere = CreateDefaultSubobject<UDrawSphereComponent>(TEXT("CommanderSphere"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetArmLength = 2000.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = false;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->CameraLagSpeed = 5.f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetWorldRotation(FRotator(-90.0f, 0.0f, 0.0f));

	CommanderCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CommanderCamera"));
	CommanderCamera->bUsePawnControlRotation = false;
	CommanderCamera->ProjectionMode = ECameraProjectionMode::Orthographic;
	CommanderCamera->OrthoWidth = 4096.0f;
	CommanderCamera->AspectRatio = 16.0f / 9.0f;
	CommanderCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	CommanderCamera->SetWorldRotation(FRotator(-90.0f, 0.0f, 0.0f));

	// Create an instance of our movement component, and tell it to update the root.
	CommanderMovementComponent = CreateDefaultSubobject<UCommanderPawnMovement>(TEXT("CommanderPawnMovement"));
	CommanderMovementComponent->UpdatedComponent = RootComponent;

}

// Called when the game starts or when spawned
void ACommanderPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACommanderPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACommanderPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("PanY", this, &ACommanderPawn::PanY);
	PlayerInputComponent->BindAxis("PanX", this, &ACommanderPawn::PanX);

}

void ACommanderPawn::PanY(float AxisValue) {
	if (CommanderMovementComponent && (CommanderMovementComponent->UpdatedComponent == RootComponent))
	{
		CommanderMovementComponent->MoveNorthSouth(AxisValue);
	}
}

void ACommanderPawn::PanX(float AxisValue) {
	if (CommanderMovementComponent && (CommanderMovementComponent->UpdatedComponent == RootComponent))
	{
		CommanderMovementComponent->MoveEastWest(AxisValue);
	}
}


