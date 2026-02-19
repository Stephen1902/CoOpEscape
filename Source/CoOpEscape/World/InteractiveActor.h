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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive Actor")
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive Actor")
	FText ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive Actor")
	FText PickUpText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive Actor")
	UTexture2D* InventoryIcon;

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
	FText Name;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
