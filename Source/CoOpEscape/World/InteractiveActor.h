// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "InteractInterface.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "InteractiveActor.generated.h"

USTRUCT()
struct FInteractiveInfo : public FTableRowBase
{
	GENERATED_BODY()

	// Name of this item.  Must match what is displayed in the corresponding actor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive Actor")
	FText ItemName;

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
};

UCLASS()
class COOPESCAPE_API AInteractiveActor : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractiveActor();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive Actor")
	USceneComponent* RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive Actor")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive Actor")
	FName Name;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UDataTable* InfoDataTable;

	virtual FName GetName_Implementation(UDataTable*& DataTableOut) const override;
};
