// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HitInterface.h"
#include "InteractibleCorpse.generated.h"

UCLASS()
class DEMOGAME_API AInteractibleCorpse : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractibleCorpse();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ApplyDamage(AActor* Dealer, float Damage) override;

	virtual FString GetPointName() override;

private:
	//class UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* CorpseMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* InteractItem;
};