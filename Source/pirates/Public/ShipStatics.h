// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ShipStatics.generated.h"

/**
 * 
 */
UCLASS()
class PIRATES_API UShipStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

		// Calculate angle difference in the range [-180, 180]
		UFUNCTION(BlueprintCallable, Category = "Math")
		static float FindDeltaAngleDegrees(float A1, float A2);

		// Finds angle between two 2D points
		UFUNCTION(BlueprintCallable, Category = "Math|Rotator", meta = (Keywords = "rotation rotate"))
		static bool FindLookAtAngle2D(const FVector2D& Start, const FVector2D& Target, float& Angle);
	
	
};
