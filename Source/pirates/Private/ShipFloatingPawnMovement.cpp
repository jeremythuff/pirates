// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipFloatingPawnMovement.h"


void UShipFloatingPawnMovement::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//UE_LOG(LogTemp, Warning, TEXT("UShipFloatingPawnMovement::Deceleration = %f"), UShipFloatingPawnMovement::Deceleration)

	// Make sure that everything is still valid, and that we are allowed to move.
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	// Move the Ship
	{		

		FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f) / DeltaTime;

		if (!DesiredMovementThisFrame.IsNearlyZero())
		{
			FHitResult Hit;
			bool safeMove = SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);

			// If we bumped into something, try to slide along it
			if (Hit.IsValidBlockingHit())
			{
				SlideAlongSurface(DesiredMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);
			}
		}
	}

}
