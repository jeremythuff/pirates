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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperSpriteComponent * MainMastSprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperSpriteComponent * CrowsnestSprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperSpriteComponent * ColorsSprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperSpriteComponent * ForemastSprite;

	/** Component shown in the editor only to indicate character facing */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* RiggingForward;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	/** The  MainMastSprite sprite when at high hitpoints */
	UPROPERTY(Category = "Rigging|MainMast", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperSprite* MainMastSpriteFullHP;

	/** The MainMastSprite sprite when at half hitpoints */
	UPROPERTY(Category = "Rigging|MainMast", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperSprite* MainMastSpriteHalfHP;

	/** The MainMastSprite sprite when at low hitpoints */
	UPROPERTY(Category = "Rigging|MainMast", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperSprite* MainMastSpriteLowHP;

	/** The MainMastSprite sprite when at no hitpoints */
	UPROPERTY(Category = "Rigging|MainMast", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperSprite* MainMastSpriteNoHP;

	/** The  ForeMastSprite sprite when at high hitpoints */
	UPROPERTY(Category = "Rigging|ForeMast", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperSprite* ForeMastSpriteFullHP;

	/** The ForeMastSprite sprite when at half hitpoints */
	UPROPERTY(Category = "Rigging|ForeMast", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperSprite*ForeMastSpriteHalfHP;

	/** The ForeMastSprite sprite when at low hitpoints */
	UPROPERTY(Category = "Rigging|ForeMast", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperSprite* ForeMastSpriteLowHP;

	/** The ForeMastSprite sprite when at no hitpoints */
	UPROPERTY(Category = "Rigging|ForeMast", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperSprite* ForeMastSpriteNoHP;
	
	UFUNCTION()
		void SetMainMastSprite(UPaperSprite* NewMainMastSprite);

	UFUNCTION()
		void SetForeMastSprite(UPaperSprite* NewFormastSprite);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Returns ArrowComponent subobject **/
	class UArrowComponent* GetRiggingForward() const { return RiggingForward; }
	
	
};
