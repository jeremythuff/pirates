// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ShipPawn.generated.h"

// Input Management
USTRUCT(BlueprintType)
struct FShipInput {
	GENERATED_BODY()

public:

	// Sanatize movement input
	UPROPERTY(Category = "Ship Input", VisibleAnywhere, BlueprintReadWrite)
	FVector2D MovementInput;

	void Sanitize();
	void MoveX(float AxisValue);
	void MoveY(float AxisValue);

	bool forward;

private:
	// Raw data
	FVector2D RawMovementInput;

};

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
		class UPaperSpriteComponent * HullSprite;

	/** The rigging actor associated with the (optional sub-object). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ship", meta = (AllowPrivateAccess = "true", MetaClass = "ARiggingActor"))
		class UChildActorComponent* ShipRigging;

	/** The Camera. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ship", meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* ShipCamera;

	/** Component shown in the editor only to indicate character facing */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ship", meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* ShipForward;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// The rotate action
	void MoveX(float AxisValue);

	// The rotate action
	void MoveY(float AxisValue);

	// The move forward action
	void MoveForward(float AxisValue);

	/** The Input Struct. */
	UPROPERTY(Category = "Ship Input", VisibleAnywhere, BlueprintReadOnly)
		FShipInput ShipInput;

	/** Maximum yaw rotation. */
	UPROPERTY(Category = "Ship", EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.0"))
		float YawSpeed;

	/** Maximum rate of movement. */
	UPROPERTY(Category = "Ship", EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.0"))
		float ShipSpeed;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/** Returns HullSprite subobject **/
	FORCEINLINE class UPaperSpriteComponent * GetHullSprite() const { return HullSprite; }

	/** Returns RiggingComponent subobject **/
	/*FORCEINLINE class UChildActorComponent* GetShipRiggingActor() const { return ShipRiggingActor; } */

	/** Returns ArrowComponent subobject **/
	class UArrowComponent* GetShipForward() const { return ShipForward; }
	
};
