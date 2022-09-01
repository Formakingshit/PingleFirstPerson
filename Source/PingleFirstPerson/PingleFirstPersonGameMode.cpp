// Copyright Epic Games, Inc. All Rights Reserved.

#include "PingleFirstPersonGameMode.h"
#include "PingleFirstPersonHUD.h"
#include "PingleFirstPersonCharacter.h"
#include "UObject/ConstructorHelpers.h"

APingleFirstPersonGameMode::APingleFirstPersonGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = APingleFirstPersonHUD::StaticClass();
}
