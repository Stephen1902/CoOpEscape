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
	
	bIsActivated = false;
	OverlapCheckTime = 0.1f;

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
	TArray<AActor*> OverlappingActors;
	TriggerBox->GetOverlappingActors(OverlappingActors);
	AActor* TriggeringActor = nullptr;

	for (int i = 0; i < OverlappingActors.Num(); ++i)
	{
		if (OverlappingActors[i]->ActorHasTag("TriggeringActor"))
		{
			TriggeringActor = OverlappingActors[i];
			break;
		}
	}

	if (TriggeringActor)
	{
		if (!bIsActivated)
		{
			bIsActivated = true;
			GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::White, TEXT("Activated"));
			OnActivatedChange.Broadcast(true);
		}
	}
	else
	{
		if (bIsActivated)
		{
			bIsActivated = false;
			GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::White, TEXT("Deactivated"));
			OnActivatedChange.Broadcast(false);
		}
	}
	
	
}
