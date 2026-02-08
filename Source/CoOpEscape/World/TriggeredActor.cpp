// Copyright 2025 DME Games

#include "TriggeredActor.h"
#include "TriggeringActor.h"

// Sets default values
ATriggeredActor::ATriggeredActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	SetRootComponent(RootComp);
	RootComp->SetIsReplicated(true);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->SetupAttachment(RootComp);
	MeshComp->SetIsReplicated(true);

	NumActorsTriggered = 0;
	bIsTriggered = false;
}

// Called when the game starts or when spawned
void ATriggeredActor::BeginPlay()
{
	Super::BeginPlay();

	SetReplicateMovement(true);

	if (HasAuthority() && TriggeringActors.Num() > 0)
	{
		for (auto It : TriggeringActors)
		{
			It->OnActivatedChange.AddDynamic(this, &ATriggeredActor::OnNumActorsChanged);
		}
	}
}

void ATriggeredActor::OnNumActorsChanged(bool ActorIncreased)
{
	if (ActorIncreased)
	{
		NumActorsTriggered = FMath::Clamp(NumActorsTriggered += 1, 0, TriggeringActors.Num());
		if (NumActorsTriggered == TriggeringActors.Num()) bIsTriggered = true;
		OnIsTriggeredTrue();
	}
	else
	{
		NumActorsTriggered = FMath::Clamp(NumActorsTriggered -= 1, 0, TriggeringActors.Num());
		bIsTriggered = false;
		OnIsTriggeredFalse();
	}
}

// Called every frame
void ATriggeredActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}


