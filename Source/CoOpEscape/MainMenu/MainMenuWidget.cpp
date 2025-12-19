// Copyright 2025 DME Games

#include "MainMenuWidget.h"

#include "Components/Button.h"
#include "CoOpEscape/CoOpGameInstanceSubsystem.h"
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
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (UCoOpGameInstanceSubsystem* GameInstanceSubsystem = GameInstance->GetSubsystem<UCoOpGameInstanceSubsystem>())
		{
			GameInstanceSubsystem->CreateServer("Create Server");
		}
	}
	
}

void UMainMenuWidget::JoinGameButtonPressed()
{
	if (const UGameInstance* GameInstance = GetGameInstance())
	{
		if (UCoOpGameInstanceSubsystem* GameInstanceSubsystem = GameInstance->GetSubsystem<UCoOpGameInstanceSubsystem>())
		{
			GameInstanceSubsystem->JoinServer("Join Server");
		}
	}
}

void UMainMenuWidget::QuitGameButtonPressed()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}
