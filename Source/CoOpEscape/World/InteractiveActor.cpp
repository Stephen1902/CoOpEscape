// Copyright 2025 DME Games

#include "InteractiveActor.h"

#include "CoOpEscape/CoOpEscapeCharacter.h"
#include "CoOpEscape/Components/InventoryComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AInteractiveActor::AInteractiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	SetRootComponent(RootComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->SetupAttachment(RootComp);
	MeshComp->SetIsReplicated(true);

	bHighlightOnOverlap = true;
/*
	static ConstructorHelpers::FObjectFinder<UDataTable> DT (TEXT("/Game/Framework/DT_InteractiveInfo"));
	if (DT.Succeeded())
	{
		InfoDataTable = DT.Object;
	}
*/
}

void AInteractiveActor::SetName(FName NameIn, ACoOpEscapeCharacter* OwnerIn)
{
	SetOwner(OwnerIn);
/*
	if (!HasAuthority())
	{
		Server_SetName(NameIn, OwnerIn);
		return;
	}
*/	
	if (!NameIn.IsNone())
	{
		Name = NameIn;

		if (InfoDataTable)
		{
			if (FInteractiveInfo* Row = InfoDataTable->FindRow<FInteractiveInfo>(Name, ""))
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
		GetWorld()->GetTimerManager().SetTimer(StopPhysicsTimer, this, &AInteractiveActor::OnTimerEnded, .75f, false, .75f);
	}
	else
	{
		Destroy();
	}
}

FName AInteractiveActor::GetInteractiveName(UDataTable*& DataTableOut) const
{
	DataTableOut = InfoDataTable;
	return LocalItemInfo.ItemName;
}

// Called when the game starts or when spawned
void AInteractiveActor::BeginPlay()
{
	Super::BeginPlay();

	SetReplicateMovement(true);
	
	if (!Name.IsNone())
	{
		SetName(Name, nullptr);
	}

	MeshComp->CustomDepthStencilValue = 1;
}
/*
FName AInteractiveActor::GetName_Implementation(UDataTable*& DataTableOut) const
{
//	IInteractInterface::GetName(DataTableOut);
	
	UE_LOG(LogTemp, Warning, TEXT("GetName Called."));
	DataTableOut = InfoDataTable;
	return Name;
}
*/
AActor* AInteractiveActor::OnOverlapBegin_Implementation(AActor* OwnerIn)
{
	IInteractInterface::OnOverlapBegin_Implementation(OwnerIn);

	if (OwnerIn)
	{
		SetOwner(OwnerIn);
	}

	if (bHighlightOnOverlap)
	{
		MeshComp->SetRenderCustomDepth(true);
	}
	
	return this;
}

void AInteractiveActor::OnOverlapEnd_Implementation()
{
	IInteractInterface::OnOverlapEnd_Implementation();
	
	if (bHighlightOnOverlap)
	{
		MeshComp->SetRenderCustomDepth(false);
	}
}

void AInteractiveActor::OnInteractBegin_Implementation()
{
	IInteractInterface::OnInteractBegin_Implementation();
	
	if (LocalSpawnType == ESpawnType::EDropped && GetOwner())
	{
		if (ACoOpEscapeCharacter* OwningChar = Cast<ACoOpEscapeCharacter>(GetOwner()))
		{
			OwningChar->GetInventoryComp()->AddToInventory(this);
		}
	}
}

void AInteractiveActor::OnTimerEnded()
{
	MeshComp->SetSimulatePhysics(false);
}

void AInteractiveActor::Server_SetName_Implementation(FName NameIn, ACoOpEscapeCharacter* OwnerIn)
{
	SetName(NameIn, OwnerIn);
}

void AInteractiveActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractiveActor, Name);
}
