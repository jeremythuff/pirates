// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipStatics.h"

float UShipStatics::FindDeltaAngleDegrees(float A1, float A2) {
	float Delta = A2 - A1;

	if (Delta > 180.0f) {
		Delta -= 360.0f;
	} else if (Delta < -180.0f) {
		Delta += 360.0f;
	}

	return Delta;
}

bool UShipStatics::FindLookAtAngle2D(const FVector2D& Start, const FVector2D& Target, float& Angle) {

	FVector2D Normal = (Target - Start).GetSafeNormal();

	bool NormalCheck = Normal.IsNearlyZero();

	if(!NormalCheck) {
		Angle = FMath::RadiansToDegrees(FMath::Atan2(Normal.Y, Normal.X));
		NormalCheck = true;
	}

	return NormalCheck;

}