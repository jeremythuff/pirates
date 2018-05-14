// Fill out your copyright notice in the Description page of Project Settings.

#include "CannonActor.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "PaperSpriteComponent.h"


// Sets default values
ACannonActor::ACannonActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CannonForward = CreateDefaultSubobject<UArrowComponent>(TEXT("CannonForward"));
	

	CannonRange = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CannonRange"));
	CannonRange->SetupAttachment(CannonForward);

	CannonSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("CannonSprite"));
	CannonSprite->SetupAttachment(CannonForward);
		//CannonSprite->SetWorldTransform(FTransform(FVector(2.0f,0.0f,2.0f)));
	CannonSprite->SetWorldRotation(FRotator(90.0f, 0.0, 0.0));
	
}

void ACannonActor::OnConstruction(const FTransform& Transform) {
	CannonRange->SetRelativeScale3D(FVector(20.0f, 20.0f, 20.0f));
	CannonSprite->SetRelativeScale3D(FVector(2.5f, 1.0f, 2.5f));
}

// Called when the game starts or when spawned
void ACannonActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACannonActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

