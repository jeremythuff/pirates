// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipPawn.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PaperSpriteComponent.h"
#include "Components/InputComponent.h"
#include "RiggingActor.h"
#include "ShipStatics.h"

FName AShipPawn::HullSpriteComponentName(TEXT("HullSprite"));
FName AShipPawn::RiggingActorName(TEXT("RiggingActor"));


void FShipInput::Sanitize() {
	MovementInput = RawMovementInput.ClampAxes(-1.0f, 1.0f);
	MovementInput = MovementInput.GetSafeNormal();
	RawMovementInput.Set(0.0f, 0.0f);
}

void FShipInput::Rotate(float AxisValue) {
	RawMovementInput.X = AxisValue;
}

void FShipInput::MoveForward(float AxisValue) {
	RawMovementInput.Y = AxisValue;
}

// Sets default values
AShipPawn::AShipPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UArrowComponent* arrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	arrowComponent->SetMobility(EComponentMobility::Movable);

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

	ShipInput.Sanitize();
	
	// Move the Ship
	{
		UE_LOG(LogTemp, Warning, TEXT("0"));
		FVector DesiredMovementDirection = FVector(ShipInput.MovementInput.X, ShipInput.MovementInput.Y, 0.0);
		if (!DesiredMovementDirection.IsNearlyZero()) {

			UE_LOG(LogTemp, Warning, TEXT("1"));
			
			// Rotate the Ship!
			FRotator MovementAngle = DesiredMovementDirection.Rotation();
			float DeltaYaw = UShipStatics::FindDeltaAngleDegrees(ShipForward->GetComponentRotation().Yaw, MovementAngle.Yaw);
			if (DeltaYaw != 0.0f) {

				UE_LOG(LogTemp, Warning, TEXT("2"));

				float MaxYawThisFrame = YawSpeed * DeltaTime;
				if (MaxYawThisFrame >= FMath::Abs(DeltaYaw)) {
					UE_LOG(LogTemp, Warning, TEXT("3"));
					ShipForward->SetWorldRotation(MovementAngle);
				}
				else {
					// Can't Reach Desired angle this frame.
					ShipForward->AddLocalRotation(FRotator(0.0f, FMath::Sign(DeltaYaw) * MaxYawThisFrame, 0.0f));
				}
			}

			// Move the Ship forward.
			FVector MovementDirection = ShipForward->GetForwardVector();
			FVector Pos = GetActorLocation();
			Pos.X += MovementDirection.X + ShipSpeed * DeltaTime;
			Pos.Y += MovementDirection.Y + ShipSpeed * DeltaTime;

			SetActorLocation(Pos);

		}
	}
}

// Called to bind functionality to input
void AShipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("Rotate", this, &AShipPawn::Rotate);
	InputComponent->BindAxis("MoveForward", this, &AShipPawn::MoveForward);

}

void AShipPawn::Rotate(float AxisValue) {

	ShipInput.Rotate(AxisValue);

}

void AShipPawn::MoveForward(float AxisValue) {

	ShipInput.MoveForward(AxisValue);

}

