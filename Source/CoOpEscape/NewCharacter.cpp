// Copyright 2025 DME Games

#include "NewCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerWidget.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InteractionComponent.h"
#include "Components/InventoryComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "World/InteractiveActor.h"

// Sets default values
ANewCharacter::ANewCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction Comp"));
	InteractionComponent->SetIsReplicated(true);
	InventoryComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory Comp"));
	InventoryComp->SetIsReplicated(true);
}

void ANewCharacter::InventoryItemChanged_Implementation(const AActor* ActorIn)
{

		/*
		if (!HasAuthority())
		{
			Server_InventoryItemChanged(ActorIn);
			return;
		}
	
		*/
		if (ActorIn)
		{
			if (UKismetSystemLibrary::DoesImplementInterface(ActorIn, UInteractInterface::StaticClass()))
			{
				UDataTable* DT;

				FName ActorName = IInteractInterface::Execute_GetName(ActorIn, DT);

				if (!ActorName.IsNone() && DT)
				{
					if (FInteractiveInfo* Row = DT->FindRow<FInteractiveInfo>(ActorName, ""))
					{
						if (PlayerWidgetRef)
						{
							UE_LOG(LogTemp, Warning, TEXT("PlayerWidgetRef is valid."));
							PlayerWidgetRef->SetIconImage(Row->InventoryIcon);
						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("PlayerWidgetRef is not valid."));
						}
						UE_LOG(LogTemp, Warning, TEXT("InventoryItemChanged called.  ActorIn and ActorName are both valid."));	
					}
				}
				else
				{
					if (ActorName.IsNone())
					{
						UE_LOG(LogTemp, Warning, TEXT("InventoryItemChanged called.  ActorIn is valid but ActorName is not."));	
					}

					if (!DT)
					{
						UE_LOG(LogTemp, Warning, TEXT("InventoryItemChanged called.  ActorIn is valid but DT is not."));
					}
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("InventoryItemChanged called.  ActorIn is not valid."));
		}
	
}

// Called when the game starts or when spawned
void ANewCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}

		if (IsLocallyControlled() && PlayerWidget)
		{
			PlayerWidgetRef = CreateWidget<UPlayerWidget>(PlayerController, PlayerWidget);
			PlayerWidgetRef->AddToViewport();
		}

		//Server_SetPlayerWidgetRef(PlayerController);
		//SetPlayerWidgetRef(PlayerController);
	}
}

void ANewCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ANewCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ANewCharacter::Interact(const FInputActionValue& Value)
{
	if (InteractionComponent)
	{
		InteractionComponent->InteractPressed();
	}
}

void ANewCharacter::Test(const FInputActionValue& Value)
{
	if (GetInventoryComp()->GetCollectableActor())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has %s as a CollectableActor"), *GetName(), *GetInventoryComp()->GetCollectableActor()->GetName());	
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory Comp does not have a valid actor"));
	}
}

// Called every frame
void ANewCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANewCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ANewCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ANewCharacter::Look);

		//Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ANewCharacter::Interact);

		//Test
		EnhancedInputComponent->BindAction(TestAction, ETriggerEvent::Started, this, &ANewCharacter::Test);
	}
}

