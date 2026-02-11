// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "InteractInterface.h"
#include "GameFramework/Actor.h"
#include "InteractiveActor.generated.h"

UCLASS()
class COOPESCAPE_API AInteractiveActor : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractiveActor();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive Actor")
	USceneComponent* RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive Actor")
	UStaticMeshComponent* MeshComp;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Interface Functions
	virtual AActor* OnOverlapBegin_Implementation() override;
	virtual void OnOverlapEnd_Implementation() override;
	virtual void OnInteractBegin_Implementation() override;
};
