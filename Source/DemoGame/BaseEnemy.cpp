// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "DemoGameCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

#define PrintString(String) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, String)

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	BaseLocation = this->GetActorLocation();
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseEnemy::SetRun(bool Run)
{
	if (Run)
	{
		GetCharacterMovement()->MaxWalkSpeed = 375.f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 150.f;
	}
}

void ABaseEnemy::DealDamage(float DamageAmount)
{
}

void ABaseEnemy::ApplyDamage(AActor* Dealer, float Damage)
{
	if (Health > 0.0)
	{
		Health = Health - Damage;
		AController* AIController = GetController<AEnemyAIController>();
		if (AIController)
		{
			AEnemyAIController* EnemyController = Cast<AEnemyAIController>(AIController);
			if (EnemyController)
			{
				EnemyController->DetectPlayer(true, Dealer);
			}
		}
	}
	else
	{
		Destroy();
	}
}

FString ABaseEnemy::GetPointName()
{
	return FString("Friends");
}

UBehaviorTree* ABaseEnemy::GetBehaviorTree()
{
	return BehaviorTree;
}

APatrolPath* ABaseEnemy::GetPatrolPath()
{
	return PatrolPath;
}

void ABaseEnemy::Attack(AActor* Enemy)
{
	if (ADemoGameCharacter* Char = Cast<ADemoGameCharacter>(Enemy))
	{
		if (Char->GetCharacterMovement()->MovementMode != MOVE_Walking)
		{
			return;
		}
	}

	if (GetCharacterMovement()->MovementMode == MOVE_Walking)
	{
		TArray<FHitResult> Hit;
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(this);
		if (UKismetSystemLibrary::SphereTraceMulti(this, 
			GetActorLocation() + (GetActorForwardVector() * 50.f),
			GetActorLocation() + (GetActorForwardVector() * 50.f), 50.f, 
			UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1),
			false, ActorsToIgnore,EDrawDebugTrace::None, Hit, true, 
			FLinearColor::Red, FLinearColor::Green, 4.f))
		{
			for (int32 i = 0; Hit.Num() > i; i++)
			{
				if (Hit[i].GetActor() == Enemy)
				{
					EnemyPlayer = Enemy;
					FVector Loc = GetActorLocation() + (GetActorForwardVector() * 80.f);
					FRotator Rot = GetActorRotation();
					Rot.Yaw += 180.0f;
					Enemy->SetActorLocationAndRotation(Loc, Rot, true);
					StopPlayer(true);
					Cast<IHitInterface>(Enemy)->ApplyDamage(this, 30.f);
					this->PlayAnimMontage(AttackAnimation);
				}
			}
		}
	}
}

bool ABaseEnemy::Interact()
{
	FHitResult Hit;
	TArray<AActor*> ActorsToIgnore;
	if (UKismetSystemLibrary::SphereTraceSingle(this,
		GetActorLocation(), GetActorLocation(), 100.f,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2), false, ActorsToIgnore,
		EDrawDebugTrace::None, Hit, true, FLinearColor::Red, FLinearColor::Green, 1.f))
	{
		if (IHitInterface* ActorInterface = Cast<IHitInterface>(Hit.GetActor()))
		{
			
			if (ActorInterface->GetPointName() == "Corpse")
			{
				bDevouringCorpse = true;
				return true;
			}
		}
	}
	return false;
}

void ABaseEnemy::Scream()
{
	if ((!bDevouringCorpse) && (GetCharacterMovement()->MovementMode == MOVE_Walking))
	{
		bScreaming = true;
	}
}

void ABaseEnemy::StopPlayer(bool Enable)
{
	if (ADemoGameCharacter* Enemy = Cast<ADemoGameCharacter>(EnemyPlayer))
	{
		if (Enable)
		{
			Enemy->StopMovement(true);
		}
		else
		{
			Enemy->StopMovement(false);
			Enemy = nullptr;
		}
	}
}