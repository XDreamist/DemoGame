// Copyright Epic Games, Inc. All Rights Reserved.

#include "DemoGameGameMode.h"
#include "DemoGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADemoGameGameMode::ADemoGameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

void ADemoGameGameMode::BeginPlay()
{
	Super::BeginPlay();
}