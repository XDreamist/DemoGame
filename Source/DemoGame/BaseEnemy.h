// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HitInterface.h"
#include "PatrolPath.h"
#include "BaseEnemy.generated.h"

UCLASS()
class DEMOGAME_API ABaseEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	APatrolPath* PatrolPath;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FVector BaseLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Health = 100.0f;

	void SetRun(bool Run);

public:
	void DealDamage(float DamageAmount);

	virtual void ApplyDamage(AActor* Dealer, float Damage) override;
	virtual FString GetPointName() override;

	UBehaviorTree* GetBehaviorTree();

	APatrolPath* GetPatrolPath();

	void Attack(AActor* Enemy);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TArray<FString> PatrolPoints;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	bool bDevouringCorpse = false;

	UFUNCTION(BlueprintCallable, Category = "Interactible")
	bool Interact();

	UFUNCTION(BlueprintCallable, Category = "Interactible")
	void Scream();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	bool bScreaming = false;

	AActor* EnemyPlayer = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StopPlayer(bool Enable);
};
