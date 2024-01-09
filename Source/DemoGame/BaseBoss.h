// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "BaseBoss.generated.h"

/**
 * 
 */
UCLASS()
class DEMOGAME_API ABaseBoss : public ABaseEnemy
{
	GENERATED_BODY()
	
public:
	ABaseBoss();

	bool LockOn(AActor* Target);

	AActor* LockedTarget = nullptr;

	FTimerHandle LockOnTimerHandle;

	void UpdateLockOn();

	void ClearLockOn();

	UFUNCTION(BlueprintCallable, Category = Combat)
	void LongRangeAttack(AActor* Target);

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ADemoGameProjectile> ProjectileClass;
};
