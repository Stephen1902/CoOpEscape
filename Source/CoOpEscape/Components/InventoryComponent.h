// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoOpEscape/World/InteractInterface.h"
#include "InventoryComponent.generated.h"

class ACoOpEscapeCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COOPESCAPE_API UInventoryComponent : public UActorComponent, public IInteractInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	void SetOwningCharacter(ACoOpEscapeCharacter* CharacterIn);

	UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
	void AddToInventory(AActor* ActorIn);

	UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
	void RemoveFromInventory(bool HasBeenPlaced);
	
	AActor* GetCollectableActor() const { return CollectableActor; }
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(Replicated)//Using=OnRep_CollectedActor)
	AActor* CollectableActor = nullptr;
	
	UFUNCTION()
	void OnRep_CollectedActor() const;

private:
	UPROPERTY()
	ACoOpEscapeCharacter* OwningCharacter = nullptr;

	UFUNCTION(Server, Reliable)
	void ServerAddToInventory(AActor* ActorIn);
	UFUNCTION(Server, Reliable)
	void Server_RemoveFromInventory(bool HasBeenPlaced);
	
	void CreateDroppedActor();
	UFUNCTION(Server, Reliable)
	void Server_CreateDroppedActor();
};
