// Copyright Epic Games, Inc. All Rights Reserved.

#include "GateSDKGameMode.h"
#include "GateSDKHUD.h"
#include "GateSDKCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGateSDKGameMode::AGateSDKGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AGateSDKHUD::StaticClass();
}
