// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ZombieScream.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "..\EnemyAIController.h"
#include "..\BaseEnemy.h"

UBTTask_ZombieScream::UBTTask_ZombieScream(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Random Zombie Scream";
}

EBTNodeResult::Type UBTTask_ZombieScream::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const cont = Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))
	{
		if (auto* const BBComp = OwnerComp.GetBlackboardComponent())
		{
			auto const SawPlayer = BBComp->GetValueAsBool(GetSelectedBlackboardKey());
			if (auto* Actor = Cast<ABaseEnemy>(cont->GetPawn()))
			{
				if (!SawPlayer)
				{
					Actor->Scream();
					BBComp->SetValueAsBool(GetSelectedBlackboardKey(), true);
				}
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}