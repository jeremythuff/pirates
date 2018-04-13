// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipPaperCharacter.h"
#include "PaperFlipbookComponent.h"

FName AShipPaperCharacter::RiggingComponentName(TEXT("Ship Rigging"));

AShipPaperCharacter::AShipPaperCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	// Try to create the ShipRigging component
	ShipRigging = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(AShipPaperCharacter::RiggingComponentName);
	if (ShipRigging)
	{
		ShipRigging->AlwaysLoadOnClient = true;
		ShipRigging->AlwaysLoadOnServer = true;
		ShipRigging->bOwnerNoSee = false;
		ShipRigging->bAffectDynamicIndirectLighting = true;
		ShipRigging->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		ShipRigging->SetupAttachment(RootComponent);
	}
}
