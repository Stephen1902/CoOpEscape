// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CoOpGameInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class COOPESCAPE_API UCoOpGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UCoOpGameInstanceSubsystem();

	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;
	
};
