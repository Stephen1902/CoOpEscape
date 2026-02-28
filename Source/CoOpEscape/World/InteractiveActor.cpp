// Copyright 2025 DME Games

#include "InteractiveActor.h"

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
	MeshComp->CustomDepthStencilValue = 1;

	bHighlightOnOverlap = true;
/*
	static ConstructorHelpers::FObjectFinder<UDataTable> DT (TEXT("/Game/Framework/DT_InteractiveInfo"));
	if (DT.Succeeded())
	{
		InfoDataTable = DT.Object;
	}
*/
}

// Called when the game starts or when spawned
void AInteractiveActor::BeginPlay()
{
	Super::BeginPlay();

	SetReplicateMovement(true);

	if (InfoDataTable)
	{
		if (FInteractiveInfo* Row = InfoDataTable->FindRow<FInteractiveInfo>(Name, ""))
		{
			MeshComp->SetStaticMesh(Row->DisplayMesh);
		}
	}
}

FName AInteractiveActor::GetName_Implementation(class UDataTable*& DataTableOut) const
{
	DataTableOut = InfoDataTable;
	return Name;
}

AActor* AInteractiveActor::OnOverlapBegin_Implementation(AActor* OwnerIn)
{
	UE_LOG(LogTemp, Warning, TEXT("OnOverlapBegin called in C++"))
	//IInteractInterface::OnOverlapBegin_Implementation(OwnerIn);
	
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
	UE_LOG(LogTemp, Warning, TEXT("OnOverlapEnd called in C++"))
	IInteractInterface::OnOverlapEnd_Implementation();
	
	if (bHighlightOnOverlap)
	{
		MeshComp->SetRenderCustomDepth(false);
	}
}

void AInteractiveActor::OnInteractBegin_Implementation()
{
	IInteractInterface::OnInteractBegin_Implementation();
}
