// Fill out your copyright notice in the Description page of Project Settings.

#include "CannonBallActor.h"
#include "Components/SphereComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"


// Sets default values
ACannonBallActor::ACannonBallActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CannonBallCollision = CreateDefaultSubobject<USphereComponent>(TEXT("CannonBallCollision"));
	CannonBallCollision->SetupAttachment(RootComponent);

	CannonBallSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("CannonBallSprite"));
	CannonBallSprite->SetupAttachment(CannonBallCollision);

	CannonBallExplosion = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(TEXT("CannonBallExplosion"));
	CannonBallExplosion->SetupAttachment(CannonBallCollision);

}

// Called when the game starts or when spawned
void ACannonBallActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACannonBallActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

