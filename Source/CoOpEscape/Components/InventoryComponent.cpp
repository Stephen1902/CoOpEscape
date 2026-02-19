// Copyright 2025 DME Games


#include "InventoryComponent.h"

#include "CoOpEscape/CoOpEscapeCharacter.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::AddToInventory(AActor* InteractiveActorIn)
{
	CollectableActor = InteractiveActorIn;

	// OnRep doesn't get called automatically on the server.  Call it.
	if (OwningCharacter && OwningCharacter->HasAuthority())
	{
		OnRep_CollectedActor();
	}
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = Cast<ACoOpEscapeCharacter>(GetOwner());
}

void UInventoryComponent::OnRep_CollectedActor()
{
	if (OwningCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s now has CollectedActor %s"), *OwningCharacter->GetName(), *CollectableActor->GetName());
		OwningCharacter->InventoryItemChanged(CollectableActor);
	}
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, CollectableActor);
}
