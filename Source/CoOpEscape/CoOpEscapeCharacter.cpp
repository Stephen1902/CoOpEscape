// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoOpEscapeCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/InteractionComponent.h"
#include "Components/InventoryComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "World/InteractiveActor.h"

//////////////////////////////////////////////////////////////////////////
// ACoOpEscapeCharacter

ACoOpEscapeCharacter::ACoOpEscapeCharacter()
{
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

void ACoOpEscapeCharacter::BeginPlay()
{
	// Call the base class  
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
			PlayerWidgetRef = CreateWidget<UPlayerWidget>(GetWorld(), PlayerWidget);
			PlayerWidgetRef->AddToViewport();
		}

		//Server_SetPlayerWidgetRef(PlayerController);
		//SetPlayerWidgetRef(PlayerController);
	}

}

//////////////////////////////////////////////////////////////////////////// Input

void ACoOpEscapeCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACoOpEscapeCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACoOpEscapeCharacter::Look);

		//Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ACoOpEscapeCharacter::Interact);

		//Test
		EnhancedInputComponent->BindAction(TestAction, ETriggerEvent::Started, this, &ACoOpEscapeCharacter::Test);
	}
}

void ACoOpEscapeCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HasAuthority())
	{
		const FString StringToDisplay =  PlayerWidgetRef ? "Valid." : "Invalid";
		GEngine->AddOnScreenDebugMessage(0, 0.f, FColor::Red, *StringToDisplay);
	}
	else
	{
		const FString StringToDisplay =  PlayerWidgetRef ? "Valid." : "Invalid";
		GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Green, *StringToDisplay);
	}
	
}

void ACoOpEscapeCharacter::InventoryItemChanged(const AActor* ActorIn)
{
	if (!PlayerWidgetRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerWidgetRef is not valid."));
	}
	else
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
						//if (IsLocallyControlled() && PlayerWidgetRef)
						{
							UE_LOG(LogTemp, Warning, TEXT("PlayerWidgetRef is valid."));
							PlayerWidgetRef->SetIconImage(Row->InventoryIcon);
						}
						//else
						//{
						//	UE_LOG(LogTemp, Warning, TEXT("PlayerWidgetRef is not valid."));
						//}
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
}

void ACoOpEscapeCharacter::Server_InventoryItemChanged_Implementation(const AActor* ActorIn)
{
	InventoryItemChanged(ActorIn);
}

void ACoOpEscapeCharacter::SetPlayerWidgetRef(UPlayerWidget* WidgetIn)
{
	PlayerWidgetRef = WidgetIn;
}

void ACoOpEscapeCharacter::Client_SetPlayerWidgetRef_Implementation(APlayerController* ControllerIn)
{
	if (PlayerWidget && ControllerIn)
	{
		PlayerWidgetRef = CreateWidget<UPlayerWidget>(ControllerIn, PlayerWidget);
		PlayerWidgetRef->AddToViewport();

		if (PlayerWidgetRef)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s has a valid PlayerWidgetRef"), HasAuthority() ? TEXT("Server") : TEXT("Client"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Widget not set in CoOpEscapeCharacter"));
	}
}

void ACoOpEscapeCharacter::Server_SetPlayerWidgetRef_Implementation(APlayerController* ControllerIn)
{
	Client_SetPlayerWidgetRef(ControllerIn);
}


void ACoOpEscapeCharacter::Move(const FInputActionValue& Value)
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

void ACoOpEscapeCharacter::Look(const FInputActionValue& Value)
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

void ACoOpEscapeCharacter::Interact(const FInputActionValue& Value)
{
	if (InteractionComponent)
	{
		InteractionComponent->InteractPressed();
	}
}

void ACoOpEscapeCharacter::Test(const FInputActionValue& Value)
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
/*
void ACoOpEscapeCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACoOpEscapeCharacter, PlayerWidgetRef);
}
*/