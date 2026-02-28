// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class COOPESCAPE_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Widget", meta=(BindWidget))
	UImage* ScreenCenterImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Widget", meta=(BindWidget))
	UImage* InteractiveIconImage;

public:
	void SetIconImage(UTexture2D* ImageIn);
};
