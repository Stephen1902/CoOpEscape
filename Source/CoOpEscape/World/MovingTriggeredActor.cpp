// Copyright 2025 DME Games

#include "MovingTriggeredActor.h"
#include "Components/ArrowComponent.h"
#include "CoOpEscape/Components/TransportComponent.h"

AMovingTriggeredActor::AMovingTriggeredActor()
{
	TransportComponent = CreateDefaultSubobject<UTransportComponent>(TEXT("Transport Comp"));

	StartLoc = CreateDefaultSubobject<UArrowComponent>(TEXT("Start Location"));
	StartLoc->SetupAttachment(RootComp);
	EndLoc = CreateDefaultSubobject<UArrowComponent>(TEXT("End Location"));
	EndLoc->SetupAttachment(RootComp);
}

void AMovingTriggeredActor::BeginPlay()
{
	Super::BeginPlay();

	SetReplicates(true);
	SetReplicateMovement(true);

	TransportComponent->SetPoints(StartLoc->GetComponentLocation(), EndLoc->GetComponentLocation());
}

void AMovingTriggeredActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	

}

void AMovingTriggeredActor::OnNumActorsChanged(bool ActorIncreased)
{
	Super::OnNumActorsChanged(ActorIncreased);
	
	TransportComponent->OnTriggeringActorActivated(bIsTriggered);
}
