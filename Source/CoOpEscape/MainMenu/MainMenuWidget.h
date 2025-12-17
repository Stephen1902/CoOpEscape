// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class COOPESCAPE_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Main Menu Widget", meta=(BindWidget))
	UButton* HostGameButton;

	UPROPERTY(EditDefaultsOnly, Category = "Main Menu Widget", meta=(BindWidget))
	UButton* JoinGameButton;

	UPROPERTY(EditDefaultsOnly, Category = "Main Menu Widget", meta=(BindWidget))
	UButton* QuitGameButton;

private:
	UFUNCTION()
	void HostGameButtonPressed();

	UFUNCTION()
	void JoinGameButtonPressed();

	UFUNCTION()
	void QuitGameButtonPressed();
};
