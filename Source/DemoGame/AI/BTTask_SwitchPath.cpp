// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SwitchPath.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../EnemyAIController.h"
#include "..\BaseEnemy.h"

#define PrintString(String) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, String)

UBTTask_SwitchPath::UBTTask_SwitchPath(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Switch Path Position";
}

EBTNodeResult::Type UBTTask_SwitchPath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const cont = Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))
	{
		if (auto* const Actor = Cast<ABaseEnemy>(cont->GetPawn()))
		{
			if (auto* const BBComp = OwnerComp.GetBlackboardComponent())
			{
				auto const NoOfPoints = Actor->GetPatrolPath()->Num();
				auto const MinIndex = 0;
				auto const MaxIndex = NoOfPoints - 1;
				auto Index = BBComp->GetValueAsInt(GetSelectedBlackboardKey());
				if (bBiDirectional)
				{
					if (Index >= MaxIndex && Direction == EDirectionType::Forward)
					{
						Direction = EDirectionType::Reverse;
					}
					else if (Index == MinIndex && Direction == EDirectionType::Reverse)
					{
						Direction = EDirectionType::Forward;
					}
				}
				BBComp->SetValueAsInt(GetSelectedBlackboardKey(), (Direction == EDirectionType::Forward ? ++Index : --Index) % NoOfPoints);

				PrintString("Success");
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}