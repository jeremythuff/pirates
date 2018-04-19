// Fill out your copyright notice in the Description page of Project Settings.

#include "RiggingActor.h"
#include "Components/ArrowComponent.h"
#include "PaperSpriteComponent.h"

// Sets default values
ARiggingActor::ARiggingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = RiggingForward = CreateDefaultSubobject<UArrowComponent>(TEXT("RiggingForward"));
	RootComponent->SetMobility(EComponentMobility::Movable);

	MainMastSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("MainMastSprite"));
	MainMastSprite->SetupAttachment(RiggingForward);

	CrowsnestSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("CrowsnestSprite"));
	CrowsnestSprite->SetupAttachment(RiggingForward);

	ColorsSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("ColorsSprite"));
	ColorsSprite->SetupAttachment(RiggingForward);

	ForemastSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("FormastSprite"));
	ForemastSprite->SetupAttachment(RiggingForward);

}

// Called when the game starts or when spawned
void ARiggingActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARiggingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

