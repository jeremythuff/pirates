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

	// Name of the Rigging Sprite Actor
	static FName RiggingActorName;

public:
	// Sets default values for this pawn's properties
	AShipPawn();

private:
	/** The main Sprite associated with the (optional sub-object). */
	UPROPERTY(Category = "Ship", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent * ShipCapsule;
	
	/** The main Sprite associated with the (optional sub-object). */
	UPROPERTY(Category = "Ship", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperSpriteComponent * HullSprite;

	/** The rigging actor associated with the (optional sub-object). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ship", meta = (AllowPrivateAccess = "true", MetaClass = "ARiggingActor"))
		class UChildActorComponent* ShipRigging;

	/** The rigging actor associated with the (optional sub-object). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ship", meta = (AllowPrivateAccess = "true", MetaClass = "UPaperFlipbook"))
		class UPaperFlipbookComponent* ShipRipple;

	/** The Camera. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ship", meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* ShipCamera;

	/** Component shown in the editor only to indicate character facing */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ship", meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* ShipForward;

	class USpringArmComponent* SpringArm;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// The rotation action
	void Turn(float AxisValue);

	// The move forward action
	void MoveForward(float AxisValue);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ship", meta = (AllowPrivateAccess = "true"))
	class UShipFloatingPawnMovement* ShipMovementComponent;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	/** Returns HullSprite subobject **/
	FORCEINLINE class UPaperSpriteComponent * GetHullSprite() const { return HullSprite; }

	virtual UPawnMovementComponent* GetMovementComponent() const override;

	/** Returns ArrowComponent subobject **/
	class UArrowComponent* GetShipForward() const { return ShipForward; }
	
};
