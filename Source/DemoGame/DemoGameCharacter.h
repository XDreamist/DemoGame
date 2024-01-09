// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "HitInterface.h"
#include "DemoGameCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class USpringArmComponent;
class UAnimMontage;
class USoundBase;
class UNiagaraComponent;

UCLASS(config=Game)
class ADemoGameCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	/** Niagara Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FX, meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* NiagaraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Dash Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* DashAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	
public:
	ADemoGameCharacter();

protected:
	virtual void BeginPlay();

public:

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** To get the Movement input value */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	FVector2D MovementVector;

	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	/** Control whethe want to Dash or not */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	bool Dashable;

	/** Enable/Disable Movement */
	UFUNCTION(BlueprintCallable, Category = Movement)
	void StopMovement(bool Stop);

	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for jumping input */
	UFUNCTION(BlueprintCallable, Category = Movement)
	void AJump();

	/** Called for Setting Dash */
	void SetDash();

	/** Called for Unsetting Dash */
	void UnsetDash();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Properties)
	float Health = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Properties)
	float DashStam = 30.0f;

	void CamShakes();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	FTimerHandle ShakeHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	TSubclassOf<class UCrosshairWidget> HUDClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI)
	class UCrosshairWidget* Crosshair;

	/** Dash Mechanics */
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float DashTime = 0.25;

	UFUNCTION(BlueprintCallable, Category = Movement)
	void Dash();

	UFUNCTION(BlueprintCallable, Category = Movement)
	void StopDash();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	FTimerHandle DashHandle;

	FTimerHandle ClearDashHandle;

	float CurrentDashStam = 30.f;

	float DashRegenValue = 10.f;

	float DashRegenTime = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	USoundBase* DashSound;

	void DashStamRegen();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	FTimerHandle DashRegenHandle;

/** Interface functions */
public:
	virtual void ApplyDamage(AActor* Dealer, float Damage) override;
	virtual FString GetPointName() override;
};