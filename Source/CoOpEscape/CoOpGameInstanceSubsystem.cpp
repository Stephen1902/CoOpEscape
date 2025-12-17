// Copyright 2025 DME Games

#include "CoOpGameInstanceSubsystem.h"

void PrintString(const FString& StringToDisplay)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, StringToDisplay);
	}
}

UCoOpGameInstanceSubsystem::UCoOpGameInstanceSubsystem()
{
	PrintString("Subsystem Construct");
}

void UCoOpGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	PrintString("Subsystem Initialize");
}

void UCoOpGameInstanceSubsystem::Deinitialize()
{
	Super::Deinitialize();

	UE_LOG(LogTemp, Warning, TEXT("Subsystem Deinitiliaze"))
}
