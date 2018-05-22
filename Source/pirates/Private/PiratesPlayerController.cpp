// Fill out your copyright notice in the Description page of Project Settings.

#include "PiratesPlayerController.h"

//Constructor
APiratesPlayerController::APiratesPlayerController()
	:
	APlayerController()
{
	APiratesPlayerController::bShowMouseCursor = true;
	APiratesPlayerController::bEnableClickEvents = true;
	APiratesPlayerController::bEnableMouseOverEvents = true;

}

void APiratesPlayerController::BeginPlay() {

	FInputModeGameAndUI inputMode;
	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	inputMode.SetHideCursorDuringCapture(false);
	APiratesPlayerController::SetInputMode(inputMode);

}

