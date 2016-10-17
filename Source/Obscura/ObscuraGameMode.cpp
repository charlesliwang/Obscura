// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Obscura.h"
#include "ObscuraGameMode.h"
#include "ObscuraCharacter.h"

AObscuraGameMode::AObscuraGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
