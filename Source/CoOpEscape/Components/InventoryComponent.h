// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COOPESCAPE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
	void AddToInventory(AActor* ActorIn);

	AActor* GetCollectableActor() const { return CollectableActor; }
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(ReplicatedUsing=OnRep_CollectedActor)
	AActor* CollectableActor;
	
	UFUNCTION()
	void OnRep_CollectedActor();

private:
	UPROPERTY()
	class ANewCharacter* OwningCharacter = nullptr;
	//class ACoOpEscapeCharacter* OwningCharacter = nullptr;
};
