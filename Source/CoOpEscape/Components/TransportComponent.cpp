// Copyright 2025 DME Games

#include "TransportComponent.h"

// Sets default values for this component's properties
UTransportComponent::UTransportComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);

	MoveTime = 2.0f;
	bArePointsSet = false;
	bIsActivated = false;
	bLockWhenOpen = false;
	bCanMove = true;
	StartPoint = FVector::Zero();
	EndPoint = FVector::Zero();
	
}


// Called when the game starts
void UTransportComponent::BeginPlay()
{
	Super::BeginPlay();

	// Owning actor must be replicated
	if (AActor* Owner = GetOwner())
	{
		Owner->SetReplicates(true);
		Owner->SetReplicateMovement(true);
	}
}

// Called every frame
void UTransportComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bCanMove)
	{
		AActor* Owner = GetOwner();
		if (bArePointsSet && Owner && Owner->HasAuthority())
		{
			FVector OwnerLoc = Owner->GetActorLocation();
			float Speed = FVector::Distance(StartPoint, EndPoint) / MoveTime;

			FVector TargetLoc = bIsActivated ? EndPoint : StartPoint;
		
			if (!OwnerLoc.Equals(TargetLoc))
			{
				FVector NewLoc = FMath::VInterpConstantTo(OwnerLoc, TargetLoc, DeltaTime, Speed);
				Owner->SetActorLocation(NewLoc);
			}
			else
			{
				if (OwnerLoc.Equals(EndPoint) && bLockWhenOpen)
				{
					bCanMove = false;;
				}
			}
		}
	}

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
	bIsActivated = ActiveState;
}
