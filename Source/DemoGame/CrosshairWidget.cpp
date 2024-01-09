// Fill out your copyright notice in the Description page of Project Settings.


#include "CrosshairWidget.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ProgressBar.h"

// This function is called when the widget is constructed.
bool UCrosshairWidget::Initialize()
{
    bool bSuccess = Super::Initialize();

    if (!bSuccess) return false;

    SetCrosshairSpread(Spread);

    return true;
}

void UCrosshairWidget::SetCrosshair(UBorder* Hair, bool vertical, FVector2D Pos)
{
    UCanvasPanelSlot* HairSlot = Cast<UCanvasPanelSlot>(Hair->Slot);
    if (vertical)
    {
        HairSlot->SetSize(FVector2D(Thickness, Length));
    }
    else
    {
        HairSlot->SetSize(FVector2D(Length, Thickness));
    }
    HairSlot->SetPosition(Pos);
}

void UCrosshairWidget::SetCrosshairSpread(float CrossSpread)
{
    SetCrosshair(Top, true, FVector2D((0.0 - Thickness) / 2.0, (0.0 - Length) - CrossSpread));
    SetCrosshair(Bottom, true, FVector2D((0.0 - Thickness) / 2.0, CrossSpread));
    SetCrosshair(Left, false, FVector2D((0.0 - Length) - CrossSpread, (0.0 - Thickness) / 2.0));
    SetCrosshair(Right, false, FVector2D(CrossSpread, (0.0 - Thickness) / 2.0));
}

void UCrosshairWidget::PlayHitAnimation()
{
    if (HitEffect)
    {
        PlayAnimation(HitEffect);
    }
}

void UCrosshairWidget::SetHealth(float CurrentHealth, float MaxHealth)
{
    if (Health)
    {
        Health->SetPercent(CurrentHealth / MaxHealth);
    }
}

void UCrosshairWidget::SetStam(float CurrentStam, float MaxStam)
{
    if (Stam)
    {
        Stam->SetPercent(CurrentStam / MaxStam);
    }
}