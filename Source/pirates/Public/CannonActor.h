// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CannonActor.generated.h"

UCLASS()
class PIRATES_API ACannonActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ACannonActor();

protected:

	/** Component shown in the editor only to indicate which way the cannon is facing */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* CannonForward;

	/** The collision component representing the cannons range*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* CannonRange;

	/** The cannon sprite */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperSpriteComponent* CannonSprite;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
