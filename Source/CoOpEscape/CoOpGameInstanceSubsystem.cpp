// Copyright 2025 DME Games

#include "CoOpGameInstanceSubsystem.h"
#include "OnlineSubsystem.h"


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
}

void UCoOpGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

//	PrintString("Subsystem Initialize");
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		FString SubsystemName = OnlineSubsystem->GetSubsystemName().ToString();
		PrintString(SubsystemName);
		
		SessionInterface = OnlineSubsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			PrintString("Session Interface is valid.");
		}
	}
}

void UCoOpGameInstanceSubsystem::Deinitialize()
{
	Super::Deinitialize();

//	UE_LOG(LogTemp, Warning, TEXT("Subsystem Deinitiliaze"))
}
