// Copyright 2026 DME Games

#include "OverlappedActor.h"
#include "Components/BoxComponent.h"

// Sets default values
AOverlappedActor::AOverlappedActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates =  true;
		
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(RootComp);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Shape"));
	TriggerBox->SetupAttachment(RootComp);
	TriggerBox->SetIsReplicated(true);
	
	DisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Display Mesh"));
	DisplayMesh->SetupAttachment(RootComp);
	DisplayMesh->SetIsReplicated(true);
	
	bIsActivated = false;
	OverlapCheckTime = 0.1f;  // 10 times per second
}

// Called when the game starts or when spawned
void AOverlappedActor::BeginPlay()
{
	Super::BeginPlay();

	SetReplicateMovement(true);

	// The events being called should only run on the server.
	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(CheckForOverlapTimer, this, &AOverlappedActor::OnOverlapTimerEnded, OverlapCheckTime, true, OverlapCheckTime);
	}
}

void AOverlappedActor::OnOverlapTimerEnded()
{
	// Get all actors overlapping this one.
	TArray<AActor*> OverlappingActors;
	TriggerBox->GetOverlappingActors(OverlappingActors);

	if (OverlappingActors.Num() > 0)
	{
		// Create an empty actor to store a found one - Using actor so non-player characters can be used ie a crate that sits on a pressure plate
		const AActor* TriggeringActor = nullptr;

		// Iterate over all overlapping actors
		for (int i = 0; i < OverlappingActors.Num(); ++i)
		{
			// Check whether the found actor can trigger this item
			if (OverlappingActors[i]->ActorHasTag("TriggeringActor"))
			{
				TriggeringActor = OverlappingActors[i];
				break;
			}
		}

		if (TriggeringActor)
		{
			// A valid actor has been found.  Check whether this actor is inactive.
			if (!bIsActivated)
			{
				// Actor is not active.  Set to active and tell actors triggered by this one it is active.
				bIsActivated = true;
				GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::White, TEXT("Activated"));
				OnActivatedChange.Broadcast(true);
			}
		}
		else
		{
			// No valid actor found.  Check whether this actor is active.
			if (bIsActivated)
			{
				// Actor is active.  Deactivate it and tell actors triggered by this it is inactive.
				bIsActivated = false;
				GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::White, TEXT("Deactivated"));
				OnActivatedChange.Broadcast(false);
			}
		}
	}
	
	
}
