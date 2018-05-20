// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CommanderPawn.generated.h"

UCLASS()
class PIRATES_API ACommanderPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACommanderPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(category = "Commander", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UDrawSphereComponent* CommanderSphere;

	UPROPERTY(category = "Commander", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	/** The Camera. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* CommanderCamera;

	/** The Ships movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Commander|Movement", meta = (AllowPrivateAccess = "true"))
		class UCommanderPawnMovement* CommanderMovementComponent;

	/** The pan y action */
	void PanY(float AxisValue);

	/** The pan x action */
	void PanX(float AxisValue);

	/** Fires when the primary selection action has begun */
	void OnBeginPrimarySelect();

	/** Fires when the primary selection action has ended */
	void OnEndPrimarySelect();

	/** Fires when the secondary selection action has begun */
	void OnBeginSecondarySelect();

	/** Fires when the secondary selection action has ended */
	void OnEndSecondarySelect();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
};
