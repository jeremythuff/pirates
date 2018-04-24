// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RiggingActor.generated.h"

UCLASS()
class PIRATES_API ARiggingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARiggingActor();

private:

	UPROPERTY(Category = "Rigging", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperSpriteComponent * MainMastSprite;

	UPROPERTY(Category = "Rigging", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperSpriteComponent * CrowsnestSprite;

	UPROPERTY(Category = "Rigging", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperSpriteComponent * ColorsSprite;

	UPROPERTY(Category = "Rigging", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperSpriteComponent * ForemastSprite;

#if WITH_EDITORONLY_DATA
	/** Component shown in the editor only to indicate character facing */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rigging", meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* RiggingForward;
#endif

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#if WITH_EDITORONLY_DATA
	/** Returns ArrowComponent subobject **/
	class UArrowComponent* GetRiggingForward() const { return RiggingForward; }
#endif
	
	
};
