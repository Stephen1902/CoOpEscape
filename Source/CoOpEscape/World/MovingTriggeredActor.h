// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "TriggeredActor.h"
#include "MovingTriggeredActor.generated.h"

class UArrowComponent;

/**
 * 
 */
UCLASS()
class COOPESCAPE_API AMovingTriggeredActor : public ATriggeredActor
{
	GENERATED_BODY()

	AMovingTriggeredActor();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Triggered Actors")
	class UTransportComponent* TransportComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Triggered Actors")
	UArrowComponent* StartTransform;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Triggered Actors")
	UArrowComponent* EndTransform;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
private:
	virtual void OnNumActorsChanged(bool ActorIncreased) override;
};
