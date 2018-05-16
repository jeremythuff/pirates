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

