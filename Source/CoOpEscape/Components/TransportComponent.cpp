// Copyright 2025 DME Games


#include "TransportComponent.h"

#include "CoOpEscape/World/TriggeringActor.h"

// Sets default values for this component's properties
UTransportComponent::UTransportComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UTransportComponent::BeginPlay()
{
	Super::BeginPlay();

	for (int32 i = 0; i < TriggeringActors.Num(); ++i)
	{
		TriggeringActors[i]->OnActivatedChange.AddDynamic(this, &UTransportComponent::OnTriggeringActorActivated);
	}
	
}


// Called every frame
void UTransportComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTransportComponent::SetPoints(FVector Point1, FVector Point2)
{
	if (Point1.Equals(Point2)) return;

	StartPoint = Point1;
	EndPoint = Point2;
	bArePointsSet = true;	
}

void UTransportComponent::OnTriggeringActorActivated(bool ActiveState)
{
	if (bArePointsSet)
	{
		if (ActiveState)
		{
			NumberOfActiveTriggers = FMath::Clamp(NumberOfActiveTriggers += 1, 0, TriggeringActors.Num());

			// Check if all triggers have been activated
			if (NumberOfActiveTriggers == TriggeringActors.Num())
			{
		
			}
		}
		else
		{
			NumberOfActiveTriggers = FMath::Clamp(NumberOfActiveTriggers -= 1, 0, TriggeringActors.Num());
		}
	}
}
