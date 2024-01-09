// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionTypes.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class DEMOGAME_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
public:
	explicit AEnemyAIController(FObjectInitializer const& ObjectInitializer);

	void DetectPlayer(bool DetectedPlayer, AActor* Player);

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	class UAISenseConfig_Sight* SightConfig;

	void SetupPerceptionSystem();

	class ABaseEnemy* Enemy;

	UFUNCTION()
	void OnSensed(AActor* Actor, FAIStimulus const Stimulus);
};
