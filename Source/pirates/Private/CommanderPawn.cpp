// Fill out your copyright notice in the Description page of Project Settings.

#include "CommanderPawn.h"

#include "Engine.h"
#include "Components/DrawSphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PiratesPlayerController.h"
#include "Camera/CameraComponent.h"
#include "PiratesMap.h"
#include "ShipPawn.h"
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
	GEngine->GameViewport->Viewport->LockMouseToViewport(true);
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

	PlayerInputComponent->BindAction("PrimarySelect", IE_Pressed, this, &ACommanderPawn::OnBeginPrimarySelect);
	PlayerInputComponent->BindAction("PrimarySelect", IE_Released, this, &ACommanderPawn::OnEndPrimarySelect);

	PlayerInputComponent->BindAction("SecondarySelect", IE_Pressed, this, &ACommanderPawn::OnBeginSecondarySelect);
	PlayerInputComponent->BindAction("SecondarySelect", IE_Released, this, &ACommanderPawn::OnEndSecondarySelect);

}

void ACommanderPawn::PanY(float AxisValue) {
	if (CommanderMovementComponent && (CommanderMovementComponent->UpdatedComponent == RootComponent)) {
		CommanderMovementComponent->MoveNorthSouth(AxisValue);
	}
}

void ACommanderPawn::PanX(float AxisValue) {
	if (CommanderMovementComponent && (CommanderMovementComponent->UpdatedComponent == RootComponent)) {
		CommanderMovementComponent->MoveEastWest(AxisValue);
	}
}

void ACommanderPawn::OnBeginPrimarySelect() {
	UE_LOG(LogTemp, Warning, TEXT("Click Left Down"));

	APiratesPlayerController* PlayerController = Cast<APiratesPlayerController>(ACommanderPawn::GetController());

	FHitResult HitResult;
	PlayerController->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, HitResult);

	if (HitResult.IsValidBlockingHit()) {
		UE_LOG(LogTemp, Warning, TEXT("Actor: %s"), *HitResult.Actor->GetName());

		AShipPawn* CastedShipPawn = Cast<AShipPawn>(HitResult.Actor);
		if (CastedShipPawn) {
			float hp = CastedShipPawn->GetHP();
			if (hp > 0.f) {
				PlayerController->SelectedActor = CastedShipPawn;
				PlayerController->PreviouslySelectedActor = this;
				PlayerController->Possess(CastedShipPawn);
			} {
				UE_LOG(LogTemp, Warning, TEXT("Ship cannot by possesed when it is destroyed."));
			}
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Actor: not valid"));
	}
}

void ACommanderPawn::OnEndPrimarySelect() {
	UE_LOG(LogTemp, Warning, TEXT("Click Left Up"));
}

void ACommanderPawn::OnBeginSecondarySelect() {

	UE_LOG(LogTemp, Warning, TEXT("Click Right Down"));

	APiratesPlayerController* PlayerController = Cast<APiratesPlayerController>(ACommanderPawn::GetController());

	FHitResult HitResult;
	PlayerController->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, HitResult);

	if (HitResult.IsValidBlockingHit()) {
		UE_LOG(LogTemp, Warning, TEXT("Actor: %s"), *HitResult.Actor->GetName());

		IPiratesMap* CastedPiratesMap = Cast<IPiratesMap>(HitResult.Actor);
		if (CastedPiratesMap)
		{
			CommanderMovementComponent->StartDrag();
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Actor: not valid"));
	}

}

void ACommanderPawn::OnEndSecondarySelect() {
	UE_LOG(LogTemp, Warning, TEXT("Click Right Up"));

	CommanderMovementComponent->StopDrag();
}


