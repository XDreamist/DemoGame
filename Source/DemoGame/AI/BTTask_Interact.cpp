// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Interact.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../EnemyAIController.h"
#include "../BaseEnemy.h"

UBTTask_Interact::UBTTask_Interact(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Interact";
}

EBTNodeResult::Type UBTTask_Interact::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const cont = Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))
	{
		if (auto* const Actor = Cast<ABaseEnemy>(cont->GetPawn()))
		{
			if (auto* const BBComp = OwnerComp.GetBlackboardComponent())
			{
				if (Actor->Interact())
				{
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}