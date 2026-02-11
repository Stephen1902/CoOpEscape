// Copyright 2025 DME Games

#include "InteractionComponent.h"
#include "CoOpEscape/CoOpEscapeCharacter.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicated(true);
}

void UInteractionComponent::InteractPressed()
{
	if (ActorBeenHit)
	{
		Execute_OnInteractBegin(ActorBeenHit);
	}
}


// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = Cast<ACoOpEscapeCharacter>(GetOwner());
	if (OwningCharacter)
	{
		GetWorld()->GetTimerManager().SetTimer(InteractiveTimer, this, &UInteractionComponent::InteractTimerExpired, TimerFrequency, true, TimerFrequency);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has no owning character."), *GetName());
	}	
}

void UInteractionComponent::InteractTimerExpired()
{
	if (!OwningCharacter || !OwningCharacter->GetController()) return;

	FVector EyesLoc;
	FRotator EyesRot;

	OwningCharacter->GetController()->GetPlayerViewPoint(EyesLoc, EyesRot);

	FVector TraceStart = EyesLoc;
	FVector TraceEnd = (EyesRot.Vector() * TraceRange) + TraceStart;
	FHitResult HitResult;

	// Check if something has been hit by a line trace
	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility))
	{
		// Check if what has been hit is different to the current actor hit
		if (HitResult.GetActor()->Implements<UInteractInterface>() && HitResult.GetActor() != ActorBeenHit)
		{
			if (ActorBeenHit != nullptr)
			{
				Execute_OnOverlapEnd(ActorBeenHit);
			}
			
			ActorBeenHit = Execute_OnOverlapBegin(HitResult.GetActor());
		}
	}
	else
	{
		// Nothing has been hit.  If there's anything previously hit, clear it
		if (ActorBeenHit != nullptr)
		{
			Execute_OnOverlapEnd(ActorBeenHit);
			ActorBeenHit = nullptr;
		}
	}
}

