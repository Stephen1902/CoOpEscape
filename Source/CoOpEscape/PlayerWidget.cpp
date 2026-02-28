// Copyright 2025 DME Games

#include "PlayerWidget.h"

#include "Components/Image.h"

void UPlayerWidget::SetIconImage(UTexture2D* ImageIn)
{
	InteractiveIconImage->SetBrushFromTexture(ImageIn);
}
