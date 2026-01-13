// Copyright 2026 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OverlappedActor.generated.h"

/**
 *  Base class for any actor in the world that is triggered directly by a player by overlapping it ie pressure plate or button
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActivatedChange, bool, NewState);

UCLASS()
class COOPESCAPE_API AOverlappedActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOverlappedActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Game Actors")
	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Game Actors")
	class UBoxComponent* TriggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Game Actors")
	UStaticMeshComponent* DisplayMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Game Actors")
	bool bIsActivated;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Game Actors")
	float OverlapCheckTime = 0.1f;

public:
	UPROPERTY()
	FOnActivatedChange OnActivatedChange;
	
private:
	FTimerHandle CheckForOverlapTimer;

	UFUNCTION()
	void OnOverlapTimerEnded();
};
