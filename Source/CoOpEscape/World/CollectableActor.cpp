// Copyright 2025 DME Games


#include "CollectableActor.h"

#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "CoOpEscape/CoOpEscapeCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ACollectableActor::ACollectableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	SetRootComponent(RootComp);

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Comp"));
	CapsuleComp->SetupAttachment(RootComp);
	CapsuleComp->SetIsReplicated(true);
	CapsuleComp->SetCollisionProfileName(FName("OverlapAllDynamic"));
	CapsuleComp->SetCapsuleHalfHeight(150.f);
	CapsuleComp->SetCapsuleRadius(100.f);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->SetupAttachment(RootComp);
	MeshComp->SetIsReplicated(true);
	MeshComp->SetCollisionProfileName(FName("OverlapAllDynamic"));

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Comp"));
	AudioComp->SetupAttachment(RootComp);
	AudioComp->SetAutoActivate(false);

	ParticleSystemComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Comp"));
	ParticleSystemComp->SetupAttachment(RootComp);
	ParticleSystemComp->SetAutoActivate(false);
	ParticleSystemComp->SetIsReplicated(true);

	RotationSpeed = 0.f;
}

// Called when the game starts or when spawned
void ACollectableActor::BeginPlay()
{
	Super::BeginPlay();
	
	SetReplicateMovement(true);

	//if (HasAuthority())
	{
		CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ACollectableActor::OnCapsuleOverlap);
	}
}

// Called every frame
void ACollectableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsCollected && RotationSpeed != 0.f)
	{
		MeshComp->AddRelativeRotation(FRotator(0.f, RotationSpeed, 0.f));
	}

}

void ACollectableActor::OnRepIsCollected()
{
	if (bIsCollected)
	{
		MeshComp->SetVisibility(false);

		if (ParticleSystemComp && ParticleSystemComp->Template)
		{
			ParticleSystemComp->Activate();
		}
			
		if (AudioComp->Sound)
		{
			AudioComp->Play();
		}
	}
}

void ACollectableActor::OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const ACoOpEscapeCharacter* Character = Cast<ACoOpEscapeCharacter>(OtherActor);
	if (Character && !bIsCollected)
	{
		bIsCollected = true;
		// ReplicatedUsing is not called automatically in c++.  Call it
		OnRepIsCollected();
	}
}

void ACollectableActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACollectableActor, bIsCollected);
}
