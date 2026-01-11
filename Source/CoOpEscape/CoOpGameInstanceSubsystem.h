// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
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

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	IOnlineSessionPtr SessionInterface;

	UFUNCTION()
	void CreateServer(const FString ServerName);

	UFUNCTION()
	void JoinSearchButtonClicked(class UMainMenuWidget* MenuRefIn);
	
	UFUNCTION()
	void JoinServerButtonClicked(int32 IndexIn);

	void SearchForSessions();

private:
	class IOnlineSubsystem* OnlineSubsystem;
	FString SubsystemName;
	FName MySessionName;

	// When searching for a server, results are stored here
	TArray<FOnlineSessionSearchResult> SearchResults;
	
	// Function to be called when the Create Session delegate has completed
	void OnCreateSessionComplete(FName SessionName, bool WasSuccessful);

	// Function to be called when the Destroy Session delegate has completed
	void OnDestroySessionComplete(FName SessionName, bool WasSuccessful);

	// Function to be called when the Find Session delegate has completed
	void OnFindSessionsComplete(bool WasSuccessful);

	// Function to be called when the Join Session delegate has completed
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type JoinResult);

	bool bCreateServerAfterDestroy;
	FString DestroyServerName;
	FString ServerNameToFind;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	UPROPERTY()
	UMainMenuWidget* MenuRef;
};
