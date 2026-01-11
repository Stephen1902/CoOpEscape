// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FoundServerWidget.generated.h"

class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class COOPESCAPE_API UFoundServerWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	UButton* ServerSelectButton;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ServerNameText;

public:
	void SetServerNameText(const FText& TextIn) const;

	void SetUp(class UMainMenuWidget* MenuRefIn, int32 IndexIn);
private:
	UFUNCTION()
	void OnServerSelectButtonClicked();

	UPROPERTY()
	UMainMenuWidget* MainMenuRef;

	uint32 Index;
};
