// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipPaperCharacter.h"
#include "PaperFlipbookComponent.h"

FName AShipPaperCharacter::RiggingComponentName(TEXT("Sprite1"));

AShipPaperCharacter::AShipPaperCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.DoNotCreateDefaultSubobject(ACharacter::MeshComponentName)) {
	// Try to create the ShipRigging component
	ShipRigging = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(AShipPaperCharacter::RiggingComponentName);

}
