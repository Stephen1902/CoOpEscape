// Copyright 2025 DME Games

#include "MainMenuPawn.h"
#include "MainMenuWidget.h"

// Sets default values
AMainMenuPawn::AMainMenuPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FClassFinder<UMainMenuWidget> MainMenu(TEXT("/Game/MainMenu/WBP_MainMenu"));
	if (MainMenu.Succeeded())
	{
		MainMenuWidgetRef = MainMenu.Class;
	}
}

// Called when the game starts or when spawned
void AMainMenuPawn::BeginPlay()
{
	Super::BeginPlay();

	if (MainMenuWidgetRef)
	{
		UMainMenuWidget* MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetRef);
		MainMenuWidget->AddToViewport();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get MainMenuWidgetRef in PlayerPawn"))
	}
}

