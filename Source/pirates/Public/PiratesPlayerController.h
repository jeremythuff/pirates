// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PiratesPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PIRATES_API APiratesPlayerController : public APlayerController
{
	GENERATED_BODY()


	
	APiratesPlayerController();

public:

	UPROPERTY()
	APawn* SelectedActor;

	UPROPERTY()
	APawn* PreviouslySelectedActor;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
