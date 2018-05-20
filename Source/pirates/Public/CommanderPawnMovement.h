// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "CommanderPawnMovement.generated.h"

/**
 * 
 */
UCLASS()
class PIRATES_API UCommanderPawnMovement : public UFloatingPawnMovement
{
	GENERATED_UCLASS_BODY()


private:

	bool Dragging = false;

	FVector2D DragOrigin;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MovementMultiplier = 1000.f;

	UFUNCTION()
		void MoveNorthSouth(float Axis);

	UFUNCTION()
		void MoveEastWest(float Axis);

	UFUNCTION()
		void StartDrag();

	UFUNCTION()
		void StopDrag();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	
};
