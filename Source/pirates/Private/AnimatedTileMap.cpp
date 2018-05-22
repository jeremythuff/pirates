// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimatedTileMap.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
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
	UPaperTileMap* TileMap = BaseTileMap->TileMap;

	BaseTileMap->MakeTileMapEditable();

	int32 MapHeight = TileMap->MapHeight;
	int32 MapWidth = TileMap->MapWidth;
	TArray<UPaperTileLayer*> Layers = TileMap->TileLayers;

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

				FString Key, Datum;
				MetadatumIter->Split(TEXT("="), &Key, &Datum);

				return Datum;

			}
		}
	}

	return TEXT("None");

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

					FString Key, Datum;
					MetadatumIter->Split(TEXT("="), &Key, &Datum);

					Metadata.AddUnique(Datum, index);

				}
			}
		}
	
	}

	return Metadata;

}

TMap<int32, FPaperTileInfo> AAnimatedTileMap::FindTileInfoAtWorldLoation(FVector WorldLocation) {

	TMap<int32, FPaperTileInfo> TilesAtLocation;
	
	UPaperTileMap* TileMap = BaseTileMap->TileMap;
	int32 MapHeight = TileMap->MapHeight;
	int32 MapWidth = TileMap->MapWidth;
	
	TArray<UPaperTileLayer*> Layers = TileMap->TileLayers;

	for (auto LayerItr(Layers.CreateIterator()); LayerItr; LayerItr++)
	{
		int32 LayerIndex = LayerItr.GetIndex();
		if (!(*LayerItr)->IsValidLowLevel()) continue;

		for (int32 TileX = 0; TileX < MapHeight; TileX++) {

			for (int32 TileY = 0; TileY < MapWidth; TileY++) {

				FVector CenterOfTile = BaseTileMap->GetTileCenterPosition(TileX, TileY, LayerIndex, true);
				
				if(FVector::DistXY(CenterOfTile, WorldLocation) < TileMap->TileWidth / 2 ) {
					FPaperTileInfo TileInfo = BaseTileMap->GetTile(TileX, TileY, LayerIndex);
					if(TileInfo.IsValid()) {
						TilesAtLocation.Add(LayerIndex, TileInfo);
					}
				}
			}
		}
	}

	return TilesAtLocation;
	
}

// Called every frame
void AAnimatedTileMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

