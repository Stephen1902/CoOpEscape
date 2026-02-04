// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CollectableActor.generated.h"

UCLASS()
class COOPESCAPE_API ACollectableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Collectable Actor")
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Collectable Actor")
	TObjectPtr<class UCapsuleComponent> CapsuleComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Collectable Actor")
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Collectable Actor")
	TObjectPtr<class UAudioComponent> AudioComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Collectable Actor")
	TObjectPtr<class UParticleSystemComponent> ParticleSystemComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Collectable Actor")
	float RotationSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing="OnRepIsCollected", Category="Collectable Actor")
	bool bIsCollected;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnRepIsCollected();

	UFUNCTION()
	void OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
