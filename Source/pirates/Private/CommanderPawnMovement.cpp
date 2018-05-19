// Fill out your copyright notice in the Description page of Project Settings.

#include "CommanderPawnMovement.h"
#include "Engine.h"
#include "CoreMinimal.h"
#include "PiratesPlayerController.h"
#include "GameFramework/Pawn.h"
#include "ShipPawn.h"

UCommanderPawnMovement::UCommanderPawnMovement(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UCommanderPawnMovement::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	

	APiratesPlayerController* PlayerController = Cast<APiratesPlayerController>(PawnOwner->GetController());
	if (PlayerController && PlayerController->IsLocalController())
	{

		

		float MousePosX, MousePosY;
		PlayerController->GetMousePosition(MousePosX, MousePosY);

		FVector2D MousePosition;
		FVector2D ViewportSize;

		//Get mouse position and screen size
		UGameViewportClient* gameViewport = GEngine->GameViewport;

		//Make sure viewport exists
		check(gameViewport);
		gameViewport->GetViewportSize(ViewportSize);

		ConsumeInputVector();

		if (gameViewport->IsFocused(gameViewport->Viewport) && gameViewport->GetMousePosition(MousePosition))
		{

			float ScreenEdgeBuffer = 25.0f;
			// Screen Edge Movement

			if (MousePosition.X < ScreenEdgeBuffer) {
				//left
				MoveEastWest(-1.f);
			} else if (ViewportSize.X - MousePosition.X < ScreenEdgeBuffer) {
				//right
				MoveEastWest(1.f);
			}

			if (MousePosition.Y < ScreenEdgeBuffer) {
				//top
				MoveNorthSouth(1.f);
			} else if (ViewportSize.Y - MousePosition.Y < ScreenEdgeBuffer) {
				//bottom
				MoveNorthSouth(-1.f);
			}

		}

		bool IsPressed;
		float TX, TY;
		PlayerController->GetInputTouchState(ETouchIndex::Touch1, TX, TY, IsPressed);
		
		if (IsPressed) {

			FHitResult HitResult;
			PlayerController->GetHitResultUnderFingerByChannel(ETouchIndex::Touch1, ETraceTypeQuery::TraceTypeQuery1, false, HitResult);
			//PlayerController->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, HitResult);

			if (HitResult.IsValidBlockingHit()) {
				UE_LOG(LogTemp, Warning, TEXT("Actor: %s"), *HitResult.Actor->GetName());

				AShipPawn* CastedShipPawn = Cast<AShipPawn>(HitResult.Actor);
				if (CastedShipPawn) {
					PlayerController->Possess(CastedShipPawn);
				}



			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("Actor: not valid"));
			}
		}

	}

}

void UCommanderPawnMovement::MoveNorthSouth(float Direction)
{

	//Don't execute any further if the camera can't move
	/*if (!bCanMoveCamera)
		return;*/

	//Calculate how much to move the camera by
	float movementValue = Direction * MovementMultiplier;

	//Create a delta vector that moves by the movementValue in the direction of the camera's yaw
	FRotator IsolatedCameraYaw = FRotator(0.0f, UpdatedComponent->GetComponentRotation().Yaw, 0.0f);
	FVector deltaMovement = movementValue * IsolatedCameraYaw.Vector();

	//Add the delta to a new vector
	FVector newLocation = UpdatedComponent->GetComponentLocation() + deltaMovement;

	//Set the new location of the pawn
	UpdatedComponent->SetWorldLocation(newLocation);

}

void UCommanderPawnMovement::MoveEastWest(float Direction)
{
	//Don't execute any further if the camera can't move
	/*if (!bCanMoveCamera)
		return;*/

	//Calculate how much to move the camera by
	float movementValue = Direction * MovementMultiplier;

	//Create a delta vector that moves by the movementValue in the direction of the right of the camera's yaw
	FRotator IsolatedCameraYaw = FRotator(0.0f, UpdatedComponent->GetComponentRotation().Yaw, 0.0f);
	FVector deltaMovement = movementValue * (FRotator(0.0f, 90.0f, 0.0f) + IsolatedCameraYaw).Vector();

	//Add the delta to a new vector
	FVector newLocation = UpdatedComponent->GetComponentLocation() + deltaMovement;

	//Set the new location of the pawn
	UpdatedComponent->SetWorldLocation(newLocation);

}
