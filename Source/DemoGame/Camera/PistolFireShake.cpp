// Fill out your copyright notice in the Description page of Project Settings.


#include "PistolFireShake.h"

UPistolFireShake::UPistolFireShake()
{
    OscillationDuration = 0.4f;
    OscillationBlendInTime = 0.35f;
    OscillationBlendOutTime = 1.0f;

    RotOscillation.Pitch.Amplitude = 4.0f; // Increased upward pitch
    RotOscillation.Pitch.Frequency = 15.0f;
    RotOscillation.Pitch.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;;

    RotOscillation.Yaw.Amplitude = 0.15f;
    RotOscillation.Yaw.Frequency = 15.0f;

    RotOscillation.Roll.Amplitude = 0.15f;
    RotOscillation.Roll.Frequency = 15.0f;

    LocOscillation.Z.Amplitude = 2.5f; // Adjusted upward location oscillation
    LocOscillation.Z.Frequency = 15.0f;
}