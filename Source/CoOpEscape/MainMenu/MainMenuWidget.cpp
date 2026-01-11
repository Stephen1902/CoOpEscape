// Copyright 2025 DME Games

#include "MainMenuWidget.h"

#include "FoundServerWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "CoOpEscape/CoOpGameInstanceSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Kismet/KismetSystemLibrary.h"

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UFoundServerWidget> ServerWidget(TEXT("/Game/MainMenu/WBP_Server"));
	if (ServerWidget.Succeeded())
	{
		ServerWidgetClass = ServerWidget.Class;
	}
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	MenuHostButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnMenuHostButtonClicked);
	MenuJoinButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnMenuJoinButtonClicked);
	MenuQuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnMenuQuitButtonClicked);
	HostBackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnHostBackButtonClicked);
	HostCreateButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnHostCreateButtonPressed);
	JoinBackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnJoinBackButtonClicked);
	JoinSearchButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnJoinSearchButtonClicked);
	JoinServerButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnJoinServerButtonClicked);

	CreateEditableText->OnTextChanged.AddDynamic(this, &UMainMenuWidget::OnCreateServerTextChanged);
}

void UMainMenuWidget::OnHostBackButtonClicked()
{
	WS_MainMenu->SetActiveWidgetIndex(0);
}

void UMainMenuWidget::OnMenuHostButtonClicked()
{
	WS_MainMenu->SetActiveWidgetIndex(1);
}

void UMainMenuWidget::OnJoinBackButtonClicked()
{
	WS_MainMenu->SetActiveWidgetIndex(0);
}

void UMainMenuWidget::OnJoinSearchButtonClicked()
{
	SearchForServers();
}

void UMainMenuWidget::OnJoinServerButtonClicked()
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (UCoOpGameInstanceSubsystem* GameInstanceSubsystem = GameInstance->GetSubsystem<UCoOpGameInstanceSubsystem>())
		{
			GameInstanceSubsystem->JoinServerButtonClicked(SelectedIndex);
		}
	} 
}

void UMainMenuWidget::OnMenuJoinButtonClicked()
{
	WS_MainMenu->SetActiveWidgetIndex(2);

	SearchForServers();
}

void UMainMenuWidget::OnMenuQuitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}

void UMainMenuWidget::OnHostCreateButtonPressed()
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (UCoOpGameInstanceSubsystem* GameInstanceSubsystem = GameInstance->GetSubsystem<UCoOpGameInstanceSubsystem>())
		{
			GameInstanceSubsystem->CreateServer(ServerName.ToString());
		}
	} 
}

void UMainMenuWidget::OnCreateServerTextChanged(const FText& Text)
{
	// Check whether the text field is empty or more than 15 characters.  If it is, disable the create server button.
	if (Text.IsEmpty() || Text.ToString().Len() > 15)
	{
		HostCreateButton->SetIsEnabled(false);

		// Advise the player of the situation
		if (Text.IsEmpty())
		{
			CreatePlayerInfo->SetText(FText::FromString("SERVER NAME CANNOT BE EMPTY"));
		}
		else
		{
			CreatePlayerInfo->SetText(FText::FromString("SERVER NAME CANNOT BE MORE THAN 16 CHARACTERS"));
		}
	}
	else
	{
		ServerName = Text;
		// Check if the button is disabled.  Enable it if it is.
		if (!HostCreateButton->GetIsEnabled())
		{
			HostCreateButton->SetIsEnabled(true);
			CreatePlayerInfo->SetText(FText::FromString(""));
		}
	}
}

void UMainMenuWidget::SearchForServers()
{
	JoinSearchButton->SetIsEnabled(false);
	JoinServerButton->SetIsEnabled(false);

	ServerSearchText->SetVisibility(ESlateVisibility::Visible);
	ServerSearchText->SetText(FText::FromString("SEARCHING FOR SERVERS"));
	
	if (ServerWidgetClass)
	{
		// Empty the scroll box so we don't get duplicates 
		JoinServerScrollBox->ClearChildren();

		if (const UGameInstance* GameInstance = GetGameInstance())
		{
			if (UCoOpGameInstanceSubsystem* GameInstanceSubsystem = GameInstance->GetSubsystem<UCoOpGameInstanceSubsystem>())
			{
				GameInstanceSubsystem->JoinSearchButtonClicked(this);
			}
		}
	}
}

void UMainMenuWidget::PopulateServerScrollBox(TArray<FOnlineSessionSearchResult> SearchResults)
{
	if (ServerWidgetClass)
	{
		ServerSearchText->SetVisibility(ESlateVisibility::Hidden);
		
		JoinSearchButton->SetIsEnabled(true);
		
		// Remove the existing list
		JoinServerScrollBox->ClearChildren();

		uint32 i = 0;
		for (FOnlineSessionSearchResult SearchResult : SearchResults)
		{
			if (SearchResult.IsValid())
			{
				UFoundServerWidget* ServerWidget = CreateWidget<UFoundServerWidget>(GetWorld(), ServerWidgetClass);
				ServerWidget->SetUp(this, i);
				++i;
				ServerWidget->SetServerNameText(FText::FromString(SearchResult.GetSessionIdStr()));
				JoinServerScrollBox->AddChild(ServerWidget);
			}
		}
	}
}

void UMainMenuWidget::FailedToFindServer()
{
	ServerSearchText->SetVisibility(ESlateVisibility::Visible);
	ServerSearchText->SetText(FText::FromString("NO SERVERS FOUND"));

	JoinSearchButton->SetIsEnabled(true);
	//JoinServerButton->SetIsEnabled(false);
}

void UMainMenuWidget::SetSelectedServerIndex(int32 IndexIn)
{
	SelectedIndex = IndexIn;

	if (IndexIn > -1)
	{
		JoinServerButton->SetIsEnabled(true);
	}

}
