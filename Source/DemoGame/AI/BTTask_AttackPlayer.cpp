// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AttackPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "..\EnemyAIController.h"
#include "..\BaseEnemy.h"

UBTTask_AttackPlayer::UBTTask_AttackPlayer(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Attack Player";
}

EBTNodeResult::Type UBTTask_AttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const cont = Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))
	{
		if (auto* const BBComp = OwnerComp.GetBlackboardComponent())
		{
			if (auto* Actor = Cast<ABaseEnemy>(cont->GetPawn()))
			{
				auto TargetObject = BBComp->GetValueAsObject(GetSelectedBlackboardKey());

				AActor* TargetActor = Cast<AActor>(TargetObject);
				if (TargetActor)
				{
					if (LongAttack)
					{
						Actor->Attack(TargetActor);
						FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
						return EBTNodeResult::Succeeded;
					}
					else
					{
						Actor->Attack(TargetActor);
						FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
						return EBTNodeResult::Succeeded;
					}
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}