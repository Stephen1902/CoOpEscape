// Copyright 2025 DME Games

#include "MainMenuGameMode.h"
#include "MainMenuPawn.h"

AMainMenuGameMode::AMainMenuGameMode()
{
	static ConstructorHelpers::FClassFinder<AMainMenuPawn> Pawn(TEXT("/Script/CoOpEscape.MainMenuPawn"));
	if (Pawn.Succeeded())
	{
		DefaultPawnClass = Pawn.Class;
	}
}