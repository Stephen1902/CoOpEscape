// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggeringActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActivatedChange, bool, NewState);

/**
 * Base class for actors that trigger another actor
 */
UCLASS()
class COOPESCAPE_API ATriggeringActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATriggeringActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Game Actors")
	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Game Actors")
	UStaticMeshComponent* DisplayMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Game Actors")
	bool bIsActivated;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(BlueprintCallable)
	FOnActivatedChange OnActivatedChange;
};
