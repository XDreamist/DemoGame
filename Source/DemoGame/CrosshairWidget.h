// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrosshairWidget.generated.h"

/**
 * 
 */
class UBorder;

UCLASS()
class DEMOGAME_API UCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	bool Initialize();

	// Croshair

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	float Length = 6.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	float Thickness = 2.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	float Spread = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UBorder* Top;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UBorder* Right;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UBorder* Left;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UBorder* Bottom;

	void SetCrosshair(UBorder* Hair, bool vertical, FVector2D Pos);

	UFUNCTION(BlueprintCallable, Category = Crosshair)
	void SetCrosshairSpread(float CrosshairSpread);

	// Others
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* BloodScreenImage;

	UFUNCTION(BlueprintCallable, Category = Combat)
	void PlayHitAnimation();

	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim))
	UWidgetAnimation* HitEffect;

	// Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* Stam;

	void SetHealth(float CurrentHealth, float MaxHealth);

	void SetStam(float CurrentStam, float MaxStam);
};
