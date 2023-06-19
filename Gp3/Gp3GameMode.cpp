// Copyright Epic Games, Inc. All Rights Reserved.

#include "Gp3GameMode.h"
#include "Gp3PlayerController.h"
#include "Gp3Character.h"
#include "UObject/ConstructorHelpers.h"

AGp3GameMode::AGp3GameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AGp3PlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}