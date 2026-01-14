// Copyright 2025 DME Games


#include "TriggeringActor.h"

// Sets default values
ATriggeringActor::ATriggeringActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates =  true;
		
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(RootComp);

	DisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Display Mesh"));
	DisplayMesh->SetupAttachment(RootComp);
	DisplayMesh->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void ATriggeringActor::BeginPlay()
{
	Super::BeginPlay();

	SetReplicateMovement(true);
}

// Called every frame
void ATriggeringActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

