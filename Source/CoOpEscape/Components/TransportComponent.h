// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TransportComponent.generated.h"

class UArrowComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COOPESCAPE_API UTransportComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTransportComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Time to move between the StartLoc and EndLoc
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transporter Comp")
	float MoveTime;

	// Whether the owned actor should lock in place when movement to the EndLoc is complete
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transporter Comp")
	bool bLockWhenOpen;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetPoints(FVector Point1, FVector Point2);

	UFUNCTION()
	void OnTriggeringActorActivated(bool ActiveState);

private:
	FVector StartPoint;
	FVector EndPoint;
	bool bArePointsSet;
	bool bIsActivated;
	bool bCanMove;	

};
