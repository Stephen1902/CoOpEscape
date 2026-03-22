// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoOpEscape/World/InteractInterface.h"
#include "InteractionComponent.generated.h"

class ACoOpEscapeCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COOPESCAPE_API UInteractionComponent : public UActorComponent, public IInteractInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionComponent();

	void SetOwningCharacter(ACoOpEscapeCharacter* CharacterIn);
	
	void InteractPressed();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Which character in the world owns this component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction Comp")
	ACoOpEscapeCharacter* OwningCharacter = nullptr;
	
	// Distance the line trace extends out of the owning character
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction Comp")
	float TraceRange = 200.f;

	// How often a check for an interactive actor should occur
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction Comp")
	float TimerFrequency = 0.1f;

private:
	FTimerHandle InteractiveTimer;

	UFUNCTION()
	void InteractTimerExpired();

	UPROPERTY(Replicated)
	AActor* ActorBeenHit = nullptr;
	void SetActorBeenHit(AActor* ActorIn);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetActorBeenHit(AActor* ActorIn);
};
