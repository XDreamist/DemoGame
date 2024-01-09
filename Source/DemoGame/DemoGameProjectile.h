// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DemoGameProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UMaterialInstance;

UCLASS(config=Game)
class ADemoGameProjectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

public:
	ADemoGameProjectile();

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }

	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	UPROPERTY(EditAnywhere, Category = Projectile)
	UMaterialInstance* BulletDecal;

	AActor* FiredActor = nullptr;

	UPROPERTY(EditAnywhere, Category = Projectile)
	class USoundBase* FleshSound;

	UPROPERTY(EditAnywhere, Category = Projectile)
	class USoundBase* DefaultSound;

	UPROPERTY(EditAnywhere, Category = Projectile)
	class UParticleSystem* FleshParticle;

	UPROPERTY(EditAnywhere, Category = Projectile)
	class UParticleSystem* DefaultParticle;
};

