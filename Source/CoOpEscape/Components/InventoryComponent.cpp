// Copyright 2025 DME Games

#include "InventoryComponent.h"
#include "CoOpEscape/CoOpEscapeCharacter.h"
#include "Net/UnrealNetwork.h"
#include "../World/InteractiveActor.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::SetOwningCharacter(ACoOpEscapeCharacter* CharacterIn)
{
	if (CharacterIn)
	{
		OwningCharacter = CharacterIn;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Set Owning Character was called on an Inventory Component but CharacterIn was null."));
	}	
}

void UInventoryComponent::AddToInventory(AActor* InteractiveActorIn)
{
	//OwningCharacter = Cast<ACoOpEscapeCharacter>(GetOwner());

	if (OwningCharacter)
	{
		if (!OwningCharacter->HasAuthority())
		{
			ServerAddToInventory(InteractiveActorIn);
			return;
		}

		CollectableActor = InteractiveActorIn;
		OwningCharacter->InventoryItemChanged(CollectableActor);
		InteractiveActorIn->Destroy();
	}
}

void UInventoryComponent::RemoveFromInventory(bool HasBeenPlaced)
{
	if (OwningCharacter && !OwningCharacter->HasAuthority())
	{
		Server_RemoveFromInventory(HasBeenPlaced);
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("RemoveFromInventory."));
	if (CollectableActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("CollectableActor Valid."));
		if (!HasBeenPlaced)
		{
			UE_LOG(LogTemp, Warning, TEXT("HasBeenPlaced is false."));
			CreateDroppedActor();
		}
		
		CollectableActor = nullptr;
		OwningCharacter->InventoryItemChanged(nullptr);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CollectableActor Invalid."));
	}
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	//OwningCharacter = Cast<ANewCharacter>(GetOwner());
}

void UInventoryComponent::OnRep_CollectedActor() const
{
	UE_LOG(LogTemp, Warning, TEXT("On Rep Called"));
	if (OwningCharacter)
	{
		OwningCharacter->InventoryItemChanged(CollectableActor);
	}
}

void UInventoryComponent::ServerAddToInventory_Implementation(AActor* ActorIn)
{
	AddToInventory(ActorIn);
}

void UInventoryComponent::Server_RemoveFromInventory_Implementation(bool HasBeenPlaced)
{
	RemoveFromInventory(HasBeenPlaced);
}

void UInventoryComponent::CreateDroppedActor()
{
	if (OwningCharacter && !OwningCharacter->HasAuthority())
	{
		Server_CreateDroppedActor();
		return;
	}

	if (CollectableActor && OwningCharacter)
	{
		UDataTable* DT;

		const FName ActorName = Cast<AInteractiveActor>(CollectableActor)->GetInteractiveName(DT);
		UE_LOG(LogTemp, Warning, TEXT("ActorName: %s."), *ActorName.ToString());

		/*
		if (UKismetSystemLibrary::DoesImplementInterface(CollectableActor, UInteractInterface::StaticClass()))
		{
			UDataTable* DT;

			const FName ActorName = Execute_GetName(CollectableActor, DT);
			UE_LOG(LogTemp, Warning, TEXT("ActorName: %s."), *ActorName.ToString());*/
			if (!ActorName.IsNone() && DT)
			{
				UE_LOG(LogTemp, Warning, TEXT("ActorName & DT."));
				if (FInteractiveInfo* Row = DT->FindRow<FInteractiveInfo>(ActorName, ""))
				{
					UE_LOG(LogTemp, Warning, TEXT("Row."));
					if (Row->SpawnType == ESpawnType::EDropped)
					{
						FVector EyesLoc;
						FRotator EyesRot;

						OwningCharacter->GetActorEyesViewPoint(EyesLoc, EyesRot);
						UE_LOG(LogTemp, Warning, TEXT("ESpawnType::Dropped."));
						const FVector ActorLocation = EyesLoc;
						const FVector ActorForwardVector = EyesRot.Vector() * 200.f;
						const FVector SpawnLocation = ActorLocation + ActorForwardVector;

						FActorSpawnParameters SpawnParameters;
						SpawnParameters.Owner = OwningCharacter;
						SpawnParameters.Instigator = OwningCharacter;
						SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

						if (AInteractiveActor* NewActor = GetWorld()->SpawnActor<AInteractiveActor>(Row->ActorToSpawn, SpawnLocation, FRotator::ZeroRotator, SpawnParameters))
						{
							NewActor->SetName(FName(Row->ItemName), OwningCharacter);
						}
					
					}					
				}
			}
		}
	}
//}

void UInventoryComponent::Server_CreateDroppedActor_Implementation()
{
	CreateDroppedActor();
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, CollectableActor);
}
