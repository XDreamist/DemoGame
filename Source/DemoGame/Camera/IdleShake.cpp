// Fill out your copyright notice in the Description page of Project Settings.


#include "IdleShake.h"

UIdleShake::UIdleShake()
{
	OscillationDuration = 0.25;

	RotOscillation.Pitch.Amplitude = 0.2;
	RotOscillation.Pitch.Frequency = 2.0;

	RotOscillation.Roll.Amplitude = 0.1;
	RotOscillation.Roll.Frequency = 1.0;

	RotOscillation.Yaw.Amplitude = 0.1;
	RotOscillation.Yaw.Frequency = 0.5;

	bSingleInstance = true;
}

