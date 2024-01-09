// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractibleCorpse.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "BaseEnemy.h"

// Sets default values
AInteractibleCorpse::AInteractibleCorpse()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    InteractItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InteractItem"));
    InteractItem->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block);
    //InteractItem->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
    InteractItem->bHiddenInGame = true;
    //InteractItem->SetupAttachment(CorpseMesh);

   

    CorpseMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CorpseMesh"));
    //CorpseMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
    CorpseMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    RootComponent = InteractItem;
    //CorpseMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -100.0f));
    

    // Initialize Capsule here similar to your previous code
    //Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCapsule"));
    //Capsule->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
    //Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    //Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block);
    //Capsule->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));

    
    // Setup attachment of Capsule to CorpseMesh
    //Capsule->SetupAttachment(CorpseMesh);
    //Capsule->InitCapsuleSize(50.0f, 100.0f);

    CorpseMesh->SetupAttachment(InteractItem);
}

// Called when the game starts or when spawned
void AInteractibleCorpse::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractibleCorpse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractibleCorpse::ApplyDamage(AActor* Dealer, float Damage)
{}

FString AInteractibleCorpse::GetPointName()
{
    return FString("Corpse");
}

