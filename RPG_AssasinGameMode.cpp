// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPG_AssasinGameMode.h"
#include "RPG_AssasinCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARPG_AssasinGameMode::ARPG_AssasinGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
