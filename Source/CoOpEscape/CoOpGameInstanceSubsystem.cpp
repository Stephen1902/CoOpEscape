// Copyright 2025 DME Games

#include "CoOpGameInstanceSubsystem.h"
#include <string>
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "MainMenu/MainMenuWidget.h"
#include "Online/OnlineSessionNames.h"

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

	OnlineSubsystem = nullptr;
	bCreateServerAfterDestroy = false;
	DestroyServerName = "";
	ServerNameToFind = "";
	MySessionName = FName("Co-op Escape Session Name");

	MenuRef = nullptr;
	
}

void UCoOpGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		SubsystemName = OnlineSubsystem->GetSubsystemName().ToString();
		SessionInterface = OnlineSubsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UCoOpGameInstanceSubsystem::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UCoOpGameInstanceSubsystem::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UCoOpGameInstanceSubsystem::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UCoOpGameInstanceSubsystem::OnJoinSessionComplete);
		}
	}
}

void UCoOpGameInstanceSubsystem::Deinitialize()
{
	Super::Deinitialize();
	
//	UE_LOG(LogTemp, Warning, TEXT("Subsystem Deinitialize"))
}

void UCoOpGameInstanceSubsystem::CreateServer(FString ServerName)
{
	if (ServerName.IsEmpty())
	{
		PrintString(TEXT("Server name cannot be empty"));
		return;
	}
	
	// Check if a session already exists, destroy it if it does
	if (const FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(MySessionName))
	{
		bCreateServerAfterDestroy = true;
		DestroyServerName = ServerName;
		SessionInterface->DestroySession(MySessionName);
		return;
	}
	
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;  // Whether to allow players to join mid-session
	SessionSettings.bIsDedicated = false;  // Whether dedicated or a listen server
	SessionSettings.bShouldAdvertise = true;  // Whether the game should advertise itself
	SessionSettings.NumPublicConnections = 2;  // The number of players that can be in the game in total
	SessionSettings.bUseLobbiesIfAvailable = true;  // Must be here for session to work
	SessionSettings.bUsesPresence = true;  // Steam uses presence.  Must be true
	SessionSettings.bAllowJoinViaPresence = true;
	if (SubsystemName == "NULL")
	{
		SessionSettings.bIsLANMatch = true;
	}
	else
	{
		SessionSettings.bIsLANMatch = false;
	}
	SessionSettings.Set(FName("DME_SERVER01"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	
	SessionInterface->CreateSession(0, MySessionName, SessionSettings);
}

void UCoOpGameInstanceSubsystem::JoinSearchButtonClicked(UMainMenuWidget* MenuRefIn)
{
	MenuRef = MenuRefIn;
 	SearchForSessions();
}

void UCoOpGameInstanceSubsystem::JoinServerButtonClicked(int32 IndexIn)
{
	if (!SessionInterface.IsValid() || !SessionSearch.IsValid()) return;

	SessionInterface->JoinSession(0, FName("DME_SERVER01"), SessionSearch->SearchResults[IndexIn]);
/*	FOnlineSessionSearchResult* CorrectResult = nullptr;
	for (FOnlineSessionSearchResult SearchResult : SearchResults)
	{
		
		if (SearchResult.IsValid())
		{
			FString FoundServerName = "DME_NOSERVER";
			SearchResult.Session.SessionSettings.Get(FName("DME_SERVER01"), FoundServerName);
			PrintString("ServerNameIn " + IndexIn.ToString() + "FoundServerName" + FoundServerName);
			if (IndexIn.ToString() == FoundServerName)
			{
				CorrectResult = &SearchResult;
				const FString Msg = FString::Printf(TEXT("Server found with name %s"), *FoundServerName);
				PrintString(Msg);
				break;
			}
		}
	}
	
	if (CorrectResult)
	{
		SessionInterface->JoinSession(0, MySessionName, *CorrectResult);
		PrintString("Found server");
	}
	else
	{
	//	OnAttemptServerJoin.Broadcast(false);
		PrintString("Couldn't find server");
	}
	*/
}

void UCoOpGameInstanceSubsystem::SearchForSessions()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
    if (SubsystemName == "NULL")
    {
    	SessionSearch->bIsLanQuery = true;
    }
    else
    {
    	SessionSearch->bIsLanQuery = false;
    }
    SessionSearch->MaxSearchResults = 128;
    SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
    	
    SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
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
	if (bWasSuccessful && bCreateServerAfterDestroy)
	{
		bCreateServerAfterDestroy = false;
		CreateServer(DestroyServerName);
	}
}

void UCoOpGameInstanceSubsystem::OnFindSessionsComplete(bool WasSuccessful)
{
	if (WasSuccessful && MenuRef)
	{
		SearchResults = SessionSearch->SearchResults;
		FString StringToDisplay = "Servers found:" + FString::FromInt(SearchResults.Num());
		PrintString(*StringToDisplay);
		
		if (SearchResults.Num() > 0)
		{
			MenuRef->PopulateServerScrollBox(SearchResults);
		}
		else
		{
			MenuRef->FailedToFindServer();
		}
	}
	else
	{
		
	}
}

void UCoOpGameInstanceSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type JoinResult)
{
	if (JoinResult == EOnJoinSessionCompleteResult::Success)
	{
		FString ServerIP = "";
		if (SessionInterface->GetResolvedConnectString(SessionName, ServerIP))
		{
			if (APlayerController* PC = GetGameInstance()->GetFirstLocalPlayerController())
			{
				PC->ClientTravel(ServerIP, TRAVEL_Absolute);
			}
		}
	}
	else
	{
		PrintString("On Join Session Complete called but failed");
	}
}

