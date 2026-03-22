// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "InteractiveActor.h"
#include "Engine/DataTable.h"
#include "PickupActor.generated.h"

UENUM()
enum class ESpawnType : uint8
{
	EDropped	UMETA(DisplayName="Dropped"),
	EPlaced		UMETA(DisplayName="Placed")
};

USTRUCT()
struct FInteractiveInfo : public FTableRowBase
{
	GENERATED_BODY()

	// Name of this item.  Must match what is displayed in the corresponding actor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive Actor")
	FName ItemName;

	// Description of this item.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive Actor")
	FText ItemDescription;

	// Display text for an on-screen widget on hover.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive Actor")
	FText PickUpText;

	// Icon to appear in a player's inventory
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive Actor")
	UTexture2D* InventoryIcon;

	// Display mesh to appear in the player's hand
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive Actor")
	UStaticMesh* DisplayMesh;

	// Actor to be spawned when the player puts this item back in the world
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive Actor")
	TSubclassOf<class AInteractiveActor> ActorToSpawn;

	// Whether this item will always spawn or will trigger a place animation on an in game actor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive Actor")
	ESpawnType SpawnType;

	// The weight of this item
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive Actor")
	float ItemWeight;

	FInteractiveInfo()
	{
		ItemName = FName("Default Name");
		ItemDescription = FText::FromString("Default Desc");
		PickUpText = FText::FromString("Default PickUpText");
		InventoryIcon = nullptr;
		DisplayMesh = nullptr;
		SpawnType = ESpawnType::EDropped;
		ItemWeight = 0.0f;
	}
};

UCLASS()
class COOPESCAPE_API APickupActor : public AInteractiveActor
{
	GENERATED_BODY()

public:
	void SetName(FName NameIn, class ACoOpEscapeCharacter* OwnerIn);
	FName GetInteractiveName(UDataTable*& DataTableOut) const;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interactive Actor")
	UDataTable* InfoDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive Actor");
	FInteractiveInfo LocalItemInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Interactive Actor")
	FName PickUpActorName;

	virtual void BeginPlay() override;
private:
	virtual AActor* OnOverlapBegin_Implementation(AActor* OwnerIn) override;
	virtual void OnOverlapEnd_Implementation() override;
	virtual void OnInteractBegin_Implementation() override;
	
	UPROPERTY()
	ESpawnType LocalSpawnType;

	UFUNCTION()
	void OnTimerEnded();
};