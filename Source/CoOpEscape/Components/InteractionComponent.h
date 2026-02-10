// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoOpEscape/World/InteractInterface.h"
#include "InteractionComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COOPESCAPE_API UInteractionComponent : public UActorComponent, public IInteractInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction Comp")
	class ACoOpEscapeCharacter* OwningCharacter = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction Comp")
	float TraceRange = 200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction Comp")
	float TimerFrequency = 0.1f;

private:
	FTimerHandle InteractiveTimer;

	UFUNCTION()
	void InteractTimerExpired();

	UPROPERTY()
	AActor* ActorBeenHit = nullptr;
};
