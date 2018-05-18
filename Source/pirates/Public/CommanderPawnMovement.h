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



public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MovementMultiplier = 1000.f;

	UFUNCTION()
		void MoveNorthSouth(float Axis);

	UFUNCTION()
		void MoveEastWest(float Axis);

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	
};