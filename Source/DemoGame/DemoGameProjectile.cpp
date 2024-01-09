// Copyright Epic Games, Inc. All Rights Reserved.

#include "DemoGameProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "HitInterface.h"
#include "Kismet/GameplayStatics.h"

ADemoGameProjectile::ADemoGameProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ADemoGameProjectile::OnHit);// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void ADemoGameProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FRotator Rotation = FRotationMatrix::MakeFromX(Hit.ImpactNormal).Rotator();
	UGameplayStatics::SpawnDecalAttached(BulletDecal, FVector(20.0, 2.5, 2.5), OtherComp, NAME_None, Hit.Location, Rotation, EAttachLocation::KeepWorldPosition, 10.0);

	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
	}

	if (OtherActor)
	{
		if (Hit.GetActor()->GetClass()->ImplementsInterface(UHitInterface::StaticClass()))
		{
			if (FiredActor)
			{
				Cast<IHitInterface>(Hit.GetActor())->ApplyDamage(FiredActor, 50.0f);

				UGameplayStatics::PlaySoundAtLocation(GetWorld(), FleshSound, Hit.Location);
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FleshParticle, Hit.Location, Rotation, true);
			}
		}
		else
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), DefaultSound, Hit.Location);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DefaultParticle, Hit.Location, Rotation, true);
		}
		Destroy();
	}
}