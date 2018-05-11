// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RiggingActor.h"
#include "Hitable.h"
#include "ShipPawn.generated.h"

UCLASS()
class PIRATES_API AShipPawn : public APawn, public IHitable
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent * ShipCapsule;
	
	/** */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ClampMin = 0, ClampMax = 100))
		int32 HP = 100;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperSpriteComponent * HullSprite;

	/** The Hull sprite when at high hitpoints */
	UPROPERTY(Category = "Ship|Hull", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperSprite* ShipHullFullHP;

	/** The Hull spritte when at half hitpoints */
	UPROPERTY(Category = "Ship|Hull", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperSprite* ShipHullHalfHP;

	/** The Hull spritte when at low hitpoints */
	UPROPERTY(Category = "Ship|Hull", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperSprite* ShipHullLowHP;

	/** The Hull spritte when at no hitpoints */
	UPROPERTY(Category = "Ship|Hull", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperSprite* ShipHullNoHP;

	UPROPERTY(Category = "Ship|Rigging", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UChildActorComponent* ShipRigging;

	/** The rigging actor associated with the (optional sub-object). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperFlipbookComponent* ShipRipple;

	/** The Camera. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* ShipCamera;

	/** Component shown in the editor only to indicate character facing */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* ShipForward;

	class USpringArmComponent* SpringArm;

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** The rotation action */
	void Turn(float AxisValue);

	/** The move forward action */
	void MoveForward(float AxisValue);

	/** Called to bind functionality to input */
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	/** The Ships movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ship|Movement", meta = (AllowPrivateAccess = "true"))
		class UShipFloatingPawnMovement* ShipMovementComponent;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	/** Returns HullSprite subobject **/
	class UPaperSpriteComponent * GetHullSprite() const { return HullSprite; }

	virtual UPawnMovementComponent* GetMovementComponent() const override;

	/** Returns ArrowComponent subobject **/
	class UArrowComponent* GetShipForward() const { return ShipForward; }
	
	/** Impl for hitable */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Hitable")
		void Hit();
		virtual void Hit_Implementation() override;
	
	/** Impl for hitable */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Hitable")
		int32 GetHP();
		virtual int32  GetHP_Implementation() override;

	UFUNCTION(BlueprintCallable)
		void UpdateRigging();

};
