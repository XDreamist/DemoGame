// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_LockOn.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "..\EnemyAIController.h"
#include "..\BaseBoss.h"

UBTTask_LockOn::UBTTask_LockOn(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Lock On Target";
}

EBTNodeResult::Type UBTTask_LockOn::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const cont = Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))
	{
		if (auto* const BBComp = OwnerComp.GetBlackboardComponent())
		{
			if (auto* Actor = Cast<ABaseBoss>(cont->GetPawn()))
			{
				auto TargetObject = BBComp->GetValueAsObject(GetSelectedBlackboardKey());

				AActor* TargetActor = Cast<AActor>(TargetObject);
				if (TargetActor)
				{
					if (Actor->LockOn(TargetActor))
					{
						FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
						return EBTNodeResult::Succeeded;
					}
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}