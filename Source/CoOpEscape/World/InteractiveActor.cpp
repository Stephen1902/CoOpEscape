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
}

// Called when the game starts or when spawned
void AInteractiveActor::BeginPlay()
{
	Super::BeginPlay();

	SetReplicateMovement(true);
}

AActor* AInteractiveActor::OnOverlapBegin_Implementation()
{
	IInteractInterface::OnInteractBegin_Implementation();
	return this;
}

void AInteractiveActor::OnOverlapEnd_Implementation()
{
	IInteractInterface::OnOverlapEnd_Implementation();
}

void AInteractiveActor::OnInteractBegin_Implementation()
{
	IInteractInterface::OnInteractBegin_Implementation();
}
