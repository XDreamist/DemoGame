// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Interact.generated.h"

/**
 * 
 */
UCLASS()
class DEMOGAME_API UBTTask_Interact : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	explicit UBTTask_Interact(FObjectInitializer const& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
