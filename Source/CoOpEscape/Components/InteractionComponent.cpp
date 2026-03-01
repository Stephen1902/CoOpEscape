// Copyright 2025 DME Games

#include "InteractionComponent.h"
#include "CoOpEscape/CoOpEscapeCharacter.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractionComponent::InteractPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("ActorBeenHit is %s"), ActorBeenHit != nullptr ? *ActorBeenHit->GetName() : TEXT("invalid"));
	if (ActorBeenHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor Been Hit is valid"));
		Execute_OnInteractBegin(ActorBeenHit);
	}
}

// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	
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
		//if (HitResult.GetActor()->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()) && HitResult.GetActor() != ActorBeenHit)
		{
			if (ActorBeenHit != nullptr)
			{
				ActorBeenHit->SetOwner(nullptr);
				Execute_OnOverlapEnd(ActorBeenHit);
			}
			UE_LOG(LogTemp, Warning, TEXT("Interaction Component has hit %s"), *HitResult.GetActor()->GetName());

			ActorBeenHit = Execute_OnOverlapBegin(HitResult.GetActor(), OwningCharacter);
			ActorBeenHit->SetOwner(OwningCharacter);
		}
	}
	else
	{
		// Nothing has been hit.  If there's anything previously hit, clear it
		if (ActorBeenHit != nullptr)
		{
			ActorBeenHit->SetOwner(nullptr);
			Execute_OnOverlapEnd(ActorBeenHit);
			ActorBeenHit = nullptr;
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("ActorBeenHit is %s"), ActorBeenHit != nullptr ? *ActorBeenHit->GetName() : TEXT("invalid"));
}

void UInteractionComponent::SetOwningCharacter(ACoOpEscapeCharacter* CharacterIn)
{
	if (CharacterIn)
	{
		OwningCharacter = CharacterIn;
		GetWorld()->GetTimerManager().SetTimer(InteractiveTimer, this, &UInteractionComponent::InteractTimerExpired, TimerFrequency, true, TimerFrequency);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Set Owning Character was called on an Interaction Component but CharacterIn was null."));
	}	
}
