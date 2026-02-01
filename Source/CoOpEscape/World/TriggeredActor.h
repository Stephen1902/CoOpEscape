// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggeredActor.generated.h"

UCLASS()
class COOPESCAPE_API ATriggeredActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATriggeredActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Triggered Actors")
	USceneComponent* RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Triggered Actors")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Triggered Actors")
	TArray<class ATriggeringActor*> TriggeringActors;

	int32 NumActorsTriggered;
	bool bIsTriggered;

	UFUNCTION()
	virtual void OnNumActorsChanged(bool ActorIncreased);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
};
