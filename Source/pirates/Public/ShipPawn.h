// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ShipPawn.generated.h"

UCLASS()
class PIRATES_API AShipPawn : public APawn
{
	GENERATED_BODY()

	// Name of the Sprite component
	static FName HullSpriteComponentName;

	// Name of the Sprite component
	static FName RiggingSpriteComponentName;

public:
	// Sets default values for this pawn's properties
	AShipPawn();

private:
	/** The main Sprite Flipbook associated with the (optional sub-object). */
	UPROPERTY(Category = "Ship", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperFlipbookComponent* HullSprite;

	/** The rigging Sprite Flipbook associated with the (optional sub-object). */
	UPROPERTY(Category = "Ship", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperFlipbookComponent* RiggingSprite;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/** Returns HullSprite subobject **/
	FORCEINLINE class UPaperFlipbookComponent* GetHullSprite() const { return HullSprite; }

	/** Returns RiggingSprite subobject **/
	FORCEINLINE class UPaperFlipbookComponent* GetRiggingSprite() const { return RiggingSprite; }
	
};
