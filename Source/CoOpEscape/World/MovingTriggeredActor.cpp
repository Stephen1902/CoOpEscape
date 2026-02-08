// Copyright 2025 DME Games

#include "MovingTriggeredActor.h"
#include "Components/ArrowComponent.h"
#include "CoOpEscape/Components/TransportComponent.h"

AMovingTriggeredActor::AMovingTriggeredActor()
{
	TransportComponent = CreateDefaultSubobject<UTransportComponent>(TEXT("Transport Comp"));
	
	StartTransform = CreateDefaultSubobject<UArrowComponent>(TEXT("Start Location"));
	StartTransform->SetupAttachment(RootComp);
	EndTransform = CreateDefaultSubobject<UArrowComponent>(TEXT("End Location"));
	EndTransform->SetupAttachment(RootComp);
}

void AMovingTriggeredActor::BeginPlay()
{
	Super::BeginPlay();
	
	TransportComponent->SetPoints(StartTransform->GetComponentTransform(), EndTransform->GetComponentTransform());
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
