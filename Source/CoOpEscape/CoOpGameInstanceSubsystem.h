// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
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

	UFUNCTION(BlueprintCallable)
	void CreateServer(FString ServerName);

	UFUNCTION(BlueprintCallable)
	void JoinServer(FString ServerName);
	
protected:
	IOnlineSessionPtr SessionInterface;
};
