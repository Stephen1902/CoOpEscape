// Copyright 2025 DME Games

#include "TransportComponent.h"

#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UTransportComponent::UTransportComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);

	MoveTime = 2.0f;
	bArePointsSet = false;
	bIsRotatingOnly = false;
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
			// Get the current transform of the owning actor 
			const FTransform OwnerTransform = Owner->GetActorTransform();
			const FTransform TargetTransform = bIsActivated ? EndTransform : StartTransform;
			
			//  Check if the owning actor is somewhere other than where it should be.
			if (!OwnerTransform.Equals(TargetTransform))
			{
				// Check if the rotation is being altered
				if (StartTransform.GetRotation().Rotator() != EndTransform.GetRotation().Rotator())
				{
					FVector StartRotXVector = UKismetMathLibrary::Conv_RotatorToVector(StartTransform.GetRotation().Rotator());
					FVector EndRotXVector = UKismetMathLibrary::Conv_RotatorToVector(EndTransform.GetRotation().Rotator());
					double StartVectorLength = UKismetMathLibrary::VSize(StartRotXVector);
					double EndVectorLength = UKismetMathLibrary::VSize(EndRotXVector);

					float Distance = FVector::Distance(StartRotXVector - EndRotXVector, FVector::Zero()) / (StartVectorLength + EndVectorLength);
					float RotationSpeed =  (Distance * 200.f) / MoveTime;
					const FRotator NewRot = FMath::RInterpConstantTo(OwnerTransform.GetRotation().Rotator(), TargetTransform.GetRotation().Rotator(), DeltaTime, RotationSpeed);
					Owner->SetActorRotation(NewRot);
				}
				
				// Check if the position is being altered
				if (StartTransform.GetLocation() != EndTransform.GetLocation())
				{
					float PositionSpeed = FVector::Distance(StartTransform.GetLocation(),  EndTransform.GetLocation()) / MoveTime;
					const FVector NewLoc = FMath::VInterpConstantTo(OwnerTransform.GetLocation(), TargetTransform.GetLocation(), DeltaTime, PositionSpeed);
					Owner->SetActorLocation(NewLoc);
				}

				// Check if the scale is being altered
				if (StartTransform.GetScale3D() != EndTransform.GetScale3D())
				{
					float ScaleSpeed = FVector::Distance(StartTransform.GetScale3D(), EndTransform.GetScale3D()) / MoveTime;
					const FVector NewScale = FMath::VInterpConstantTo(OwnerTransform.GetScale3D(), TargetTransform.GetScale3D(), DeltaTime, ScaleSpeed);
					Owner->SetActorScale3D(NewScale);
				}
			}
		}
	}
}

void UTransportComponent::SetPoints(FTransform Point1, FTransform Point2)
{
	if (Point1.Equals(Point2)) return;

	StartTransform = Point1;
	EndTransform = Point2;
	
	StartPoint = Point1.GetLocation();
	EndPoint = Point2.GetLocation();

	StartRot = Point1.GetRotation().Rotator();
	EndRot = Point1.GetRotation().Rotator();
	
	bArePointsSet = true;	
}

void UTransportComponent::OnTriggeringActorActivated(bool ActiveState)
{
	bIsActivated = ActiveState;
}
