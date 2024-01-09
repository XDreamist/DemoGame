// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GenerateLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "../EnemyAIController.h"
#include "../BaseEnemy.h"

UBTTask_GenerateLocation::UBTTask_GenerateLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Random Location In NavMesh";
}

EBTNodeResult::Type UBTTask_GenerateLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const contr = Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))
	{
		if (auto* const enemy = contr->GetPawn())
		{
			auto const Origin = enemy->GetActorLocation();
			if (auto* Actor = Cast<ABaseEnemy>(enemy))
			{
				if (auto* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
				{
					FNavLocation Loc;
					if (NavSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, Loc))
					{
						Actor->SetRun(false);
						OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Loc.Location);

						FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
						return EBTNodeResult::Succeeded;
					}
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}
