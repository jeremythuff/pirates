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

public:
	// Sets default values for this pawn's properties
	AShipPawn();

private:
	/** The main Sprite Flipbook associated with the (optional sub-object). */
	UPROPERTY(Category = "Ship", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperSpriteComponent * HullSprite;

#if WITH_EDITORONLY_DATA
	/** Component shown in the editor only to indicate character facing */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ship", meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* ShipForward;
#endif

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/** Returns HullSprite subobject **/
	FORCEINLINE class UPaperSpriteComponent * GetHullSprite() const { return HullSprite; }

#if WITH_EDITORONLY_DATA
	/** Returns ArrowComponent subobject **/
	class UArrowComponent* GetShipForward() const { return ShipForward; }
#endif
	
};
