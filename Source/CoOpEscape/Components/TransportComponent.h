// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TransportComponent.generated.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transporter Comp")
	UArrowComponent* StartLoc;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transporter Comp")
	UArrowComponent* EndLoc;

	// Time to move between the StartLoc and EndLoc
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transporter Comp")
	float MoveTime;
	
	// Other actors that trigger this one
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transporter Comp")
	TArray<class ATriggeringActor*> TriggeringActors;

	// Whether this transporter comp is used by the actor it is attached to
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transporter Comp")
	bool bUseTransportComp;

	UPROPERTY(VisibleAnywhere, Category = "Transporter Comp")
	int32 ActivatedTriggerCount;

	UPROPERTY(VisibleAnywhere, Category = "Transporter Comp")
	bool bAllTriggersActivated;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetPoints(FVector Point1, FVector Point2);
	
private:
	FVector StartPoint;
	FVector EndPoint;
	bool bArePointsSet;
	
	int32 NumberOfActiveTriggers;
	UFUNCTION()
	void OnTriggeringActorActivated(bool ActiveState);
};
