// Copyright Epic Games, Inc. All Rights Reserved.

#include "DemoGameCharacter.h"
#include "DemoGameProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/IdleShake.h"
#include "Camera/SprintShake.h"
#include "NiagaraComponent.h"
#include "Blueprint/UserWidget.h"
#include "CrosshairWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#define PrintString(...) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, FString::Printf(__VA_ARGS__))

//////////////////////////////////////////////////////////////////////////
// ADemoGameCharacter

ADemoGameCharacter::ADemoGameCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a Spring Arm Component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->SetRelativeLocation(FVector(-10.f, 0.f, 60.f));
	SpringArm->TargetArmLength = 0.0f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 25.0f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(SpringArm);
	FirstPersonCameraComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = false;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SprintFX"));
	NiagaraComponent->SetupAttachment(RootComponent);

	Crosshair = nullptr;
	HUDClass = nullptr;
}

void ADemoGameCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
		Crosshair = CreateWidget<UCrosshairWidget>(PlayerController, HUDClass);
		if (Crosshair)
		{
			Crosshair->AddToPlayerScreen();
		}
	}
	GetWorldTimerManager().SetTimer(ShakeHandle, this, &ADemoGameCharacter::CamShakes, 0.01f, true, 0.0f);
}

//////////////////////////////////////////////////////////////////////////// Input

void ADemoGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ADemoGameCharacter::AJump);
		/*EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping); */

		//Dashing
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &ADemoGameCharacter::SetDash);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Completed, this, &ADemoGameCharacter::UnsetDash);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADemoGameCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADemoGameCharacter::Look);
	}
}

void ADemoGameCharacter::AJump()
{
	ADemoGameCharacter::Jump();
}

void ADemoGameCharacter::CamShakes()
{
	FVector VelocityXY = GetVelocity();
	VelocityXY.Z = 0.0f;

	if (VelocityXY.Length() > 0.0)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(USprintShake::StaticClass());
	}
	else
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(UIdleShake::StaticClass());
	}
}

void ADemoGameCharacter::SetDash()
{
	Dashable = true;
}

void ADemoGameCharacter::UnsetDash()
{
	Dashable = false;
}

void ADemoGameCharacter::Dash()
{
	if ((GetCharacterMovement()->MovementMode == MOVE_Walking) || (GetCharacterMovement()->MovementMode == MOVE_Falling))
	{
		if (CurrentDashStam >= 10.f)
		{
			NiagaraComponent->SetVectorParameter(FName("DashDirection"), FVector(500.0f * abs(MovementVector.Y), 500.0f * MovementVector.X, 0.0f));

			float DashDistance = 1000.0f;
			float Alpha = 0.0f;
			float AlphaIncrease = 0.032f;

			FVector ForwardDashValue = GetActorLocation() + (GetActorForwardVector() * (MovementVector.Y * DashDistance));
			FVector RightDashValue = GetActorLocation() + (GetActorRightVector() * (MovementVector.X * DashDistance));

			SetActorLocation(FMath::Lerp(GetActorLocation(), ForwardDashValue, (Alpha + AlphaIncrease)), true);
			SetActorLocation(FMath::Lerp(GetActorLocation(), RightDashValue, (Alpha = Alpha + AlphaIncrease)), true);

			NiagaraComponent->SetVisibility(true);
		}
	}
}

void ADemoGameCharacter::StopDash()
{
	if (CurrentDashStam >= 10.f)
	{
		CurrentDashStam -= 10.f;
		Crosshair->SetStam(CurrentDashStam, DashStam);
	}
	GetWorldTimerManager().ClearTimer(DashHandle);
	NiagaraComponent->SetVisibility(false);

	if (DashRegenHandle.IsValid())
	{
		GetWorld()->GetTimerManager().UnPauseTimer(DashRegenHandle);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(DashRegenHandle, this, &ADemoGameCharacter::DashStamRegen, DashRegenTime, true, DashRegenTime);
	}
}

void ADemoGameCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
	
	if (Dashable)
	{
		GetWorldTimerManager().SetTimer(DashHandle, this, &ADemoGameCharacter::Dash, 0.01f, true, 0.0f);
		if (CurrentDashStam >= 10.f)
		{
			UGameplayStatics::PlaySoundAtLocation(this, DashSound, GetActorLocation());
		}
		GetWorldTimerManager().SetTimer(ClearDashHandle, this, &ADemoGameCharacter::StopDash, 0.01f, false, DashTime);
	}
}

void ADemoGameCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ADemoGameCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool ADemoGameCharacter::GetHasRifle()
{
	return bHasRifle;
}

void ADemoGameCharacter::DashStamRegen()
{
	if (CurrentDashStam < DashStam)
	{
		CurrentDashStam = CurrentDashStam + DashRegenValue;
		Crosshair->SetStam(CurrentDashStam, DashStam);
	}
	else
	{
		CurrentDashStam = DashStam;
		GetWorld()->GetTimerManager().PauseTimer(DashRegenHandle);
	}
}

void ADemoGameCharacter::ApplyDamage(AActor* Dealer, float Damage)
{
	if (Dealer == this)
	{
		return;
	}
	if (Health > 0.0)
	{
		Health = Health - Damage;
		if (Crosshair)
		{
			Crosshair->PlayHitAnimation();
			Crosshair->SetHealth(Health, 100.f);
		}
	}
	else
	{
		Destroy();
	}
}

FString ADemoGameCharacter::GetPointName()
{
	return FString("Enemies");
}

void ADemoGameCharacter::StopMovement(bool Stop)
{
	if (Stop)
	{
		GetCharacterMovement()->DisableMovement();
	}
	else
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
}