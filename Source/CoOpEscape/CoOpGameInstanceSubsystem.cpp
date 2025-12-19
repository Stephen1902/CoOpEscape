// Copyright 2025 DME Games

#include "CoOpGameInstanceSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

void PrintString(const FString& StringToDisplay)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, StringToDisplay);
	}
}

UCoOpGameInstanceSubsystem::UCoOpGameInstanceSubsystem()
{
//	PrintString("Subsystem Construct");

	bCreateServerAfterDestroy = false;
}

void UCoOpGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

//	PrintString("Subsystem Initialize");
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		SessionInterface = OnlineSubsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UCoOpGameInstanceSubsystem::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UCoOpGameInstanceSubsystem::OnDestroySessionComplete);
		}
	}
}

void UCoOpGameInstanceSubsystem::Deinitialize()
{
	Super::Deinitialize();
	
//	UE_LOG(LogTemp, Warning, TEXT("Subsystem Deinitiliaze"))
}

void UCoOpGameInstanceSubsystem::CreateServer(FString ServerName)
{
	if (ServerName.IsEmpty())
	{
		PrintString("Server Name cannot be empty.");
		return;
	}

	LocalServerName = ServerName;
	FName MySessionName = FName("Co-Op Session Name");

	// Check if the session name already exists, destroy it if it does
	if (SessionInterface->GetNamedSession(MySessionName))
	{
		PrintString("Session exists.  Destroying.");
		bCreateServerAfterDestroy = true;
		SessionInterface->DestroySession(MySessionName);
		return;
	}

	FOnlineSessionSettings MySessionSettings;
	MySessionSettings.bAllowJoinInProgress = true;
	MySessionSettings.bIsDedicated = false;
	MySessionSettings.bShouldAdvertise = true;
	MySessionSettings.NumPublicConnections = 2;
	MySessionSettings.bUseLobbiesIfAvailable = true;
	MySessionSettings.bUsesPresence = true;
	MySessionSettings.bAllowJoinViaPresence = true;

	// Check whether the game is using the Steam API or not and set LAN match accordingly
	FString SubsystemName = IOnlineSubsystem::Get()->GetSubsystemName().ToString();
	MySessionSettings.bIsLANMatch = SubsystemName == "NULL" ? true : false;
	
	SessionInterface->CreateSession(0, MySessionName, MySessionSettings);
}

void UCoOpGameInstanceSubsystem::JoinServer(FString ServerName)
{
	PrintString(ServerName);
}

void UCoOpGameInstanceSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	PrintString(FString::Printf(TEXT("OnCreateSessionComplete %d"), bWasSuccessful));
	
	if (bWasSuccessful)
	{
		GetWorld()->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");
	}
}

void UCoOpGameInstanceSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		if (bCreateServerAfterDestroy)
		{
			bCreateServerAfterDestroy = false;
			CreateServer(LocalServerName);
		}
	}
}
