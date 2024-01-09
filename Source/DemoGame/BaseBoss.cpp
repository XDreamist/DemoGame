// Fill out your copyright notice in the Description page of Project Settings.

#include "DemoGameProjectile.h"
#include "BaseBoss.h"

ABaseBoss::ABaseBoss()
{

}

bool ABaseBoss::LockOn(AActor* Target)
{
    if (Target && Target != this)
    {
        LockedTarget = Target;
        GetWorldTimerManager().SetTimer(LockOnTimerHandle, this, &ABaseBoss::UpdateLockOn, 0.01f, true);
        return true;
    }
    return false;
}

void ABaseBoss::UpdateLockOn()
{
    if (LockedTarget)
    {
        float MaxLockOnDistance = 20000.0f;

        float Distance = (LockedTarget->GetActorLocation() - GetActorLocation()).Size();
        if (Distance < MaxLockOnDistance)
        {
            FVector TargetDirection = LockedTarget->GetActorLocation() - GetActorLocation();
            TargetDirection.Z = 0.0f;
            TargetDirection.Normalize();

            // Rotate the character towards the target direction
            FRotator TargetRotation = TargetDirection.Rotation();
            SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), 25.f));
        }
    }
}

void ABaseBoss::ClearLockOn()
{
    LockedTarget = nullptr;
    // Additional logic to clear any lock-on related states or visuals
}

void ABaseBoss::LongRangeAttack(AActor* Target)
{
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			const FRotator SpawnRotation = GetActorRotation();
            const FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 50.f;

			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			ADemoGameProjectile* Bullet = World->SpawnActor<ADemoGameProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			if (Bullet)
			{
				Bullet->FiredActor = this;
			}
		}
	}
}