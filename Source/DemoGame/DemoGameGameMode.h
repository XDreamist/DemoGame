// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DemoGameGameMode.generated.h"

UCLASS(minimalapi)
class ADemoGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADemoGameGameMode();

protected:
	virtual void BeginPlay();

};



