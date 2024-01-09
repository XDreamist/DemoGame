// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DemoGameCharacter.h"
#include "BaseEnemy.h"

AEnemyAIController::AEnemyAIController(FObjectInitializer const& ObjectInitializer)
{
	SetupPerceptionSystem();
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	Enemy = Cast<ABaseEnemy>(InPawn);
	if (Enemy)
	{
		if (UBehaviorTree* const BehaviorTree = Enemy->GetBehaviorTree())
		{
			UBlackboardComponent* BBComponent;
			UseBlackboard(BehaviorTree->BlackboardAsset, BBComponent);
			Blackboard = BBComponent;
			RunBehaviorTree(BehaviorTree);

			if ((Enemy->PatrolPoints.Num()))
			{
				GetBlackboardComponent()->SetValueAsBool("RandOrFixed", true);
			}
			else
			{
				GetBlackboardComponent()->SetValueAsBool("RandOrFixed", false);
			}
		}
	}
}

void AEnemyAIController::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>
			(TEXT("Perception Component")));
		SightConfig->SightRadius = 1250.f;
		SightConfig->LoseSightRadius = 1285.f;
		SightConfig->PeripheralVisionAngleDegrees = 60.f;
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		SightConfig->SetMaxAge(0.1f);

		GetPerceptionComponent()->SetDominantSense(SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this,
			&AEnemyAIController::OnSensed);
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}
}

void AEnemyAIController::OnSensed(AActor* Actor, FAIStimulus const Stimulus)
{
	if (auto* const SensedChar = Cast<ADemoGameCharacter>(Actor))
	{
		DetectPlayer(Stimulus.WasSuccessfullySensed(), SensedChar);
	}
}

void AEnemyAIController::DetectPlayer(bool DetectedPlayer, AActor* Player)
{
	GetBlackboardComponent()->SetValueAsBool("PlayerDetected", DetectedPlayer);
	GetBlackboardComponent()->SetValueAsObject("DetectedPlayer", Player);
	Enemy->SetRun(DetectedPlayer);
	Enemy->bDevouringCorpse = false;
}