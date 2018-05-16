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
	MaxSpeed = 16000.f;
	Acceleration = 5000.f;
	Deceleration = 4000.f;
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

		const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

		float ScreenEdgeBuffer = 25.0f;
		// Screen Edge Movement
		
		if (MousePosX && MousePosX < 10.0f && MousePosX >= 0.0f) {
			//left
			UCommanderPawnMovement::AddInputVector(FVector(0.0f, -1000.f, 0.0f));
		}

		if (MousePosX && MousePosX > (ViewportSize.X - ScreenEdgeBuffer) && MousePosX <= ViewportSize.X) {
			//right
			UCommanderPawnMovement::AddInputVector(FVector(0.0f, 1000.f, 0.0f));
		}
		
		
		
		if (MousePosY && MousePosY < 10.0f && MousePosY >= 0.0f) {
			//top
			UCommanderPawnMovement::AddInputVector(FVector(1000.f, 0.0f, 0.0f));
		}

		if (MousePosY && MousePosY > (ViewportSize.Y - ScreenEdgeBuffer) && MousePosY <= ViewportSize.Y) {
			//bottom
			UCommanderPawnMovement::AddInputVector(FVector(-1000.f, 0.0f, 0.0f));
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
