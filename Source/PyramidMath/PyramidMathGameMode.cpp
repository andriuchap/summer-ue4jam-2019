// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PyramidMathGameMode.h"
#include "PyramidMathCharacter.h"
#include "ExplorerHUD.h"
#include "UObject/ConstructorHelpers.h"

APyramidMathGameMode::APyramidMathGameMode(const FObjectInitializer &ObjInitializer)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	HUDClass = AExplorerHUD::StaticClass();
}
