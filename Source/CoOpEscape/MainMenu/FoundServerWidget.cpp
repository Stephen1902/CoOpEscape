// Copyright 2025 DME Games

#include "FoundServerWidget.h"
#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UFoundServerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UFoundServerWidget::SetServerNameText(const FText& TextIn) const
{
	ServerNameText->SetText(TextIn);
}

void UFoundServerWidget::SetUp(UMainMenuWidget* MenuRefIn, int32 IndexIn)
{
	MainMenuRef = MenuRefIn;
	Index = IndexIn;

	ServerSelectButton->OnClicked.AddDynamic(this, &UFoundServerWidget::OnServerSelectButtonClicked);
}

void UFoundServerWidget::OnServerSelectButtonClicked()
{
	if (MainMenuRef)
	{
		if (GEngine)
		{
			FString StringToDisplay = "ServerNameText is " + ServerNameText->GetText().ToString(); 
			GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Green, *StringToDisplay);
		}
		MainMenuRef->SetSelectedServerIndex(Index);
	}
	else
	{
		if (GEngine)
		{
			FString StringToDisplay = "MainMenuRef Is not valid."; 
			GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Green, *StringToDisplay);
		}
	}
}
