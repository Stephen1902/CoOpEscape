// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "InteractInterface.h"
#include "CoOpEscape/CoOpEscapeCharacter.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "InteractiveActor.generated.h"

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

	FInteractiveInfo()
	{
		ItemName = FName("Default Name");
		ItemDescription = FText::FromString("Default Desc");
		PickUpText = FText::FromString("Default PickUpText");
		InventoryIcon = nullptr;
		DisplayMesh = nullptr;
		SpawnType = ESpawnType::EDropped;
	}
};

UCLASS()
class COOPESCAPE_API AInteractiveActor : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractiveActor();

	void SetName(FName NameIn, ACoOpEscapeCharacter* OwnerIn);
	FName GetInteractiveName(UDataTable*& DataTableOut) const;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive Actor")
	USceneComponent* RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive Actor")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Replicated, Category = "Interactive Actor")
	FName Name;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Interactive Actor")
	bool bHighlightOnOverlap;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interactive Actor")
	UDataTable* InfoDataTable;

private:
	//virtual FName GetName_Implementation(UDataTable*& DataTableOut) const override;
	virtual AActor* OnOverlapBegin_Implementation(AActor* OwnerIn) override;
	virtual void OnOverlapEnd_Implementation() override;
	virtual void OnInteractBegin_Implementation() override;

	UPROPERTY()
	ESpawnType LocalSpawnType;

	UFUNCTION()
	void OnTimerEnded();

	UFUNCTION(Server, Reliable)
	void Server_SetName(FName NameIn, ACoOpEscapeCharacter* OwnerIn);
};
