// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "CoOpGameInstanceSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttemptServerJoin, bool, Successful);
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

	UFUNCTION(BlueprintCallable, Category = "Multiplayer Subsystem")
	void CreateServer(const FString ServerName);

	UFUNCTION(BlueprintCallable, Category = "Multiplayer Subsystem")
	void JoinServer(const FString ServerName);

	UPROPERTY(BlueprintAssignable)
	FOnAttemptServerJoin OnAttemptServerJoin;

private:
	class IOnlineSubsystem* OnlineSubsystem;
	FString SubsystemName;
	FName MySessionName;

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
};
