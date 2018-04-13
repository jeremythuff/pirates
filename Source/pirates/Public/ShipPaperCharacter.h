// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "ShipPaperCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PIRATES_API AShipPaperCharacter : public APaperCharacter
{
	GENERATED_UCLASS_BODY()

	static FName RiggingComponentName;

	private:


	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UPaperFlipbookComponent* ShipRigging;

	public:

		FORCEINLINE class UPaperFlipbookComponent* GetShipRigging() const { return ShipRigging; }
};
