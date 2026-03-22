// Copyright 2025 DME Games

#include "PickupActor.h"
#include "CoOpEscape/CoOpEscapeCharacter.h"
#include "CoOpEscape/Components/InventoryComponent.h"
#include "Net/UnrealNetwork.h"

void APickupActor::SetName(FName NameIn, ACoOpEscapeCharacter* OwnerIn)
{
	SetOwner(OwnerIn);

	if (!NameIn.IsNone())
	{
		PickUpActorName = NameIn;

		if (InfoDataTable)
		{
			if (FInteractiveInfo* Row = InfoDataTable->FindRow<FInteractiveInfo>(PickUpActorName, ""))
			{
				MeshComp->SetStaticMesh(Row->DisplayMesh);

				LocalItemInfo.ItemName = Row->ItemName;
				LocalItemInfo.ItemDescription = Row->ItemDescription;
				LocalItemInfo.PickUpText = Row->PickUpText;
				LocalItemInfo.DisplayMesh = Row->DisplayMesh;
				LocalItemInfo.InventoryIcon = Row->InventoryIcon;
				LocalItemInfo.SpawnType = Row->SpawnType;
				LocalItemInfo.ActorToSpawn = Row->ActorToSpawn;
				LocalItemInfo.ItemWeight = Row->ItemWeight;
			}
		}

		MeshComp->SetSimulatePhysics(true);
		FTimerHandle StopPhysicsTimer;
		GetWorld()->GetTimerManager().SetTimer(StopPhysicsTimer, this, &APickupActor::OnTimerEnded, .75f, false, .75f);
	}
	else
	{
		Destroy();
	}
}

FName APickupActor::GetInteractiveName(UDataTable*& DataTableOut) const
{
	DataTableOut = InfoDataTable;
	return LocalItemInfo.ItemName;
}

void APickupActor::BeginPlay()
{
	Super::BeginPlay();

	if (!PickUpActorName.IsNone())
	{
		SetName(PickUpActorName, nullptr);
	}
}

AActor* APickupActor::OnOverlapBegin_Implementation(AActor* OwnerIn)
{
	Super::OnOverlapBegin_Implementation(OwnerIn);
	
	return this;
}

void APickupActor::OnOverlapEnd_Implementation()
{
	Super::OnOverlapEnd_Implementation();
}

void APickupActor::OnTimerEnded()
{
	MeshComp->SetSimulatePhysics(false);
}

void APickupActor::OnInteractBegin_Implementation()
{
	IInteractInterface::OnInteractBegin_Implementation();
	
	if (LocalItemInfo.SpawnType == ESpawnType::EDropped && GetOwner())
	{
		if (ACoOpEscapeCharacter* OwningChar = Cast<ACoOpEscapeCharacter>(GetOwner()))
		{
			OwningChar->GetInventoryComp()->AddToInventory(this);
		}
	}
}

void APickupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APickupActor, PickUpActorName);
}
