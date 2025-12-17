// Copyright 2025 DME Games

#include "MainMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HostGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HostGameButtonPressed);
	JoinGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::JoinGameButtonPressed);
	QuitGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::QuitGameButtonPressed);
}

void UMainMenuWidget::HostGameButtonPressed()
{
	
}

void UMainMenuWidget::JoinGameButtonPressed()
{
	
}

void UMainMenuWidget::QuitGameButtonPressed()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}
