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
		
	}
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, CollectableActor);
}
