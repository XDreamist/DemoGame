// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPathLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "..\HitInterface.h"
#include "..\EnemyAIController.h"
#include "..\BaseEnemy.h"

#define PrintString(String) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, String)

UBTTask_FindPathLocation::UBTTask_FindPathLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find Patrol Path";
}

EBTNodeResult::Type UBTTask_FindPathLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const cont = Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))
	{
		if (auto* const BBComp = OwnerComp.GetBlackboardComponent())
		{
			auto const index = BBComp->GetValueAsInt(GetSelectedBlackboardKey());

			if (auto* Actor = Cast<ABaseEnemy>(cont->GetPawn()))
			{
				if (FindRestPoint)
				{
					TArray<FHitResult> Hit;
					TArray<AActor*> ActorsToIgnore;
					if (UKismetSystemLibrary::SphereTraceMulti(Actor,
						Actor->GetActorLocation(), Actor->GetActorLocation(), 1200.f, 
						UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2), false, ActorsToIgnore, 
						EDrawDebugTrace::None, Hit, true, FLinearColor::Red, FLinearColor::Green, 1.f))
					{
						for (const FHitResult& HitResult : Hit)
						{
							if (auto* HitActor = HitResult.GetActor())
							{
								if (auto* HitInterface = Cast<IHitInterface>(HitActor))
								{
									if (index < Actor->PatrolPoints.Num())
									{
										FString Point = Actor->PatrolPoints[index];
										if (HitInterface->GetPointName() == Point)
										{
											Actor->SetRun(false);
											BBComp->SetValueAsVector("MoveToLocation", HitActor->GetActorLocation());
											FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
											return EBTNodeResult::Succeeded;
										}
									}
									else
									{
										BBComp->SetValueAsInt("Index", 0);
									}
								}
							}
						}
					}
				}
				else
				{
					Actor->SetRun(false);
					auto const Point = Actor->GetPatrolPath()->GetPatrolPoints(index);

					auto const GlobalPoint = Actor->GetPatrolPath()->GetActorTransform().TransformPosition(Point);

					BBComp->SetValueAsVector("MoveToLocation", GlobalPoint);

					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}