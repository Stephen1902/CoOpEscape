// Copyright 2026 DME Games

#pragma once

#include "CoreMinimal.h"
#include "TriggeringActor.h"
#include "GameFramework/Actor.h"
#include "OverlappingActor.generated.h"

/**
 *  Base class for any actor in the world that is triggered directly by a player by overlapping it ie pressure plate or button
 */

UCLASS()
class COOPESCAPE_API AOverlappingActor : public ATriggeringActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOverlappingActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Game Actors")
	class UBoxComponent* TriggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Game Actors")
	float OverlapCheckTime = 0.1f;
	
private:
	FTimerHandle CheckForOverlapTimer;

	UFUNCTION()
	void OnOverlapTimerEnded();
};
