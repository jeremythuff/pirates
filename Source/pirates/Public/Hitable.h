// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Hitable.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UHitable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PIRATES_API IHitable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//classes using this interface must implement Hit
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Hitable")
		void Hit(); 

	//classes using this interface must implement Hit
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Hitable")
		int32 GetHP(); 
	
};
