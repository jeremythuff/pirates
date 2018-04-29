// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimatedTileMap.h"
#include "PaperFlipBook.h"
#include "PaperFlipBookComponent.h"
#include "PaperTileMap.h"
#include "PaperTileSet.h"
#include "PaperTileMapComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"



// Sets default values
AAnimatedTileMap::AAnimatedTileMap()
{
	
	if (!RootComponent) {
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Animated Map"));
	}

	NorthArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("NorthArrow"));
	NorthArrow->SetupAttachment(RootComponent);
	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	BaseTileMap = CreateOptionalDefaultSubobject<UPaperTileMapComponent>(TEXT("BaseTileMap"));
	BaseTileMap->SetupAttachment(NorthArrow);

}

// Called when the game starts or when spawned
void AAnimatedTileMap::BeginPlay()
{
	Super::BeginPlay();
	
	AAnimatedTileMap::SetUpMapAnimation();

}

void AAnimatedTileMap::SetUpMapAnimation() {
	UPaperTileMap* TMap = BaseTileMap->TileMap;

	BaseTileMap->MakeTileMapEditable();

	int32 MapHeight = TMap->MapHeight;
	int32 MapWidth = TMap->MapWidth;
	TArray<UPaperTileLayer*> Layers = TMap->TileLayers;

	for (auto LayerItr(Layers.CreateIterator()); LayerItr; LayerItr++)
	{

		int32 layerZ = LayerItr.GetIndex();
		if (!(*LayerItr)->IsValidLowLevel()) continue;

		for (int32 tileX = 0; tileX < MapHeight; tileX++) {

			for (int32 tileY = 0; tileY < MapWidth; tileY++) {

				FString FlipbookName = ExtractTileUserData(tileX, tileY, LayerItr.GetIndex(), TEXT("flipbook"));

				for (auto AnimatedTilesItr(AnimatedTiles.CreateIterator()); AnimatedTilesItr; AnimatedTilesItr++)
				{

					if (FlipbookName.Equals((*AnimatedTilesItr)->GetName())) {

						// shut off current tile, but retain collision.
						BaseTileMap->SetTile(tileX, tileY, layerZ, FPaperTileInfo());

						// place flipbook in the same spot where the current tile is.
						FVector location = BaseTileMap->GetTileCenterPosition(tileX, tileY, layerZ);
						FRotator rotate = BaseTileMap->GetComponentRotation();

						UPaperFlipbookComponent* NewFlipbook = NewObject<UPaperFlipbookComponent>(BaseTileMap);
						NewFlipbook->RegisterComponent();

						NewFlipbook->SetFlipbook(AnimatedTiles[AnimatedTilesItr.GetIndex()]);
						NewFlipbook->SetRelativeLocation(location);
						NewFlipbook->AttachToComponent(BaseTileMap, FAttachmentTransformRules::KeepRelativeTransform);
						NewFlipbook->PlayFromStart();
					}
				}

			}

		}

	}

}

FString AAnimatedTileMap::ExtractTileUserData(int32 TileX, int32 TileY, int32 LayerIndex, FString MetadataLabel) {

	FPaperTileInfo TileInfo = BaseTileMap->GetTile(TileX, TileY, LayerIndex);
	UPaperTileSet* TileSet = TileInfo.TileSet;
	FString TileUserData = TileSet->GetTileUserData(TileInfo.PackedTileIndex).ToString();

	TileUserData.RemoveSpacesInline();
	if (TileUserData.Contains(MetadataLabel)) {

		TArray<FString> Metadatum;
		TileUserData.ParseIntoArray(Metadatum, TEXT(";"), true);

		for (auto MetadatumIter(Metadatum.CreateIterator()); MetadatumIter; MetadatumIter++)
		{

			if (MetadatumIter->Contains(MetadataLabel)) {

				FString Key, Metadatum;
				MetadatumIter->Split(TEXT("="), &Key, &Metadatum);

				return Metadatum;

			}
		}
	}

	return TEXT("NO_Name");

}

TMultiMap<FString, int32> AAnimatedTileMap::ExtractAllTileUserData(FString MetadataLabel) {
	
	TSoftObjectPtr<UPaperTileSet> TileSet = BaseTileMap->TileMap->SelectedTileSet;

	TMultiMap<FString, int32> Metadata;

	for (int32 index = 0; index < TileSet->GetTileCount(); index++) {
	
		FString TileUserData = TileSet->GetTileUserData(index).ToString();

		TileUserData.RemoveSpacesInline();
		if (TileUserData.Contains(MetadataLabel)) {

			TArray<FString> Metadatum;
			TileUserData.ParseIntoArray(Metadatum, TEXT(";"), true);

			for (auto MetadatumIter(Metadatum.CreateIterator()); MetadatumIter; MetadatumIter++)
			{

				if (MetadatumIter->Contains(MetadataLabel)) {

					FString Key, Metadatum;
					MetadatumIter->Split(TEXT("="), &Key, &Metadatum);

					Metadata.AddUnique(Metadatum, index);

				}
			}
		}
	
	}

	return Metadata;

}

// Called every frame
void AAnimatedTileMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

