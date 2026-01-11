// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UWidgetSwitcher;
class UCanvasPanel;
class UButton;
class UTextBlock;
class UEditableText;
class UScrollBox;

/**
 * 
 */
UCLASS()
class COOPESCAPE_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

	UMainMenuWidget(const FObjectInitializer & ObjectInitializer);
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, Category = "Main Menu Widget", meta=(BindWidget))
	UWidgetSwitcher* WS_MainMenu;
	
	UPROPERTY(EditDefaultsOnly, Category = "Main Menu Widget", meta=(BindWidget))
	UCanvasPanel* CP_MainMenu;
	
	UPROPERTY(EditDefaultsOnly, Category = "Main Menu Widget", meta=(BindWidget))
	UButton* MenuHostButton;

	UPROPERTY(EditDefaultsOnly, Category = "Main Menu Widget", meta=(BindWidget))
	UButton* MenuJoinButton;

	UPROPERTY(EditDefaultsOnly, Category = "Main Menu Widget", meta=(BindWidget))
	UButton* MenuQuitButton;

	UPROPERTY(EditDefaultsOnly, Category = "Main Menu Widget", meta=(BindWidget))
	UCanvasPanel* CP_CreateMenu;

	UPROPERTY(EditDefaultsOnly, Category = "Main Menu Widget", meta=(BindWidget))
	UEditableText* CreateEditableText;

	UPROPERTY(EditDefaultsOnly, Category = "Main Menu Widget", meta=(BindWidget))
	UTextBlock* CreatePlayerInfo;
	
	UPROPERTY(EditDefaultsOnly, Category = "Main Menu Widget", meta=(BindWidget))
	UButton* HostCreateButton;
	
	UPROPERTY(EditDefaultsOnly, Category = "Main Menu Widget", meta=(BindWidget))
	UButton* HostBackButton;

	UPROPERTY(EditDefaultsOnly, Category = "Main Menu Widget", meta=(BindWidget))
	UCanvasPanel* CP_JoinMenu;

	UPROPERTY(EditDefaultsOnly, Category = "Main Menu Widget", meta=(BindWidget))
	UScrollBox* JoinServerScrollBox;

	UPROPERTY(EditDefaultsOnly, Category = "Main Menu Widget", meta=(BindWidget))
	UTextBlock* JoinPlayerInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Main Menu Widget", meta=(BindWidget))
	UButton* JoinSearchButton;
	
	UPROPERTY(EditDefaultsOnly, Category = "Main Menu Widget", meta=(BindWidget))
	UButton* JoinServerButton;
	
	UPROPERTY(EditDefaultsOnly, Category = "Main Menu Widget", meta=(BindWidget))
	UButton* JoinBackButton;

	UPROPERTY(EditDefaultsOnly, Category = "Main Menu Widget", meta=(BindWidget))
	UTextBlock* ServerSearchText;
private:
	// Main Menu Functions 
	UFUNCTION()
	void OnMenuHostButtonClicked();

	UFUNCTION()
    void OnMenuJoinButtonClicked();

	UFUNCTION()
    void OnMenuQuitButtonClicked();
	
	// Host Menu Functions
	UFUNCTION()
	void OnHostBackButtonClicked();

	UFUNCTION()
    void OnHostCreateButtonPressed();

	UFUNCTION()
    void OnCreateServerTextChanged(const FText& Text);
	
	// Join Menu Functions
	UFUNCTION()
	void OnJoinBackButtonClicked();
	
	UFUNCTION()
	void OnJoinSearchButtonClicked();

	UFUNCTION()
	void OnJoinServerButtonClicked();
	
	FText ServerName;

	UPROPERTY()
	TSubclassOf<UUserWidget> ServerWidgetClass;

	void SearchForServers();

	int32 SelectedIndex;
public:
	void PopulateServerScrollBox(TArray<FOnlineSessionSearchResult> SearchResults );

	void FailedToFindServer();

	void SetSelectedServerIndex(int32 IndexIn);
};
