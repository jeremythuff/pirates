// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "PaperTileMap.h"
#include "PaperTileSet.h"

#include "PiratesMap.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UPiratesMap : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PIRATES_API IPiratesMap
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="PiratesMap")
	  TMap<int32, FPaperTileInfo> FindTileInfoAtWorldLoation(FVector WorldLocation);


};
