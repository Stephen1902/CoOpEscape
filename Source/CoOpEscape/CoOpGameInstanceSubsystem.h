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

private:
	bool bCreateServerAfterDestroy;
	
	FString LocalServerName;
	
	// Function to be called when the Create Session delegate has completed
	UFUNCTION()
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	// Function to be called when the Destroy Session delegate has completed
	UFUNCTION()
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
};
