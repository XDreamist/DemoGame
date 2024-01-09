// Fill out your copyright notice in the Description page of Project Settings.


#include "SprintShake.h"

USprintShake::USprintShake()
{
	OscillationDuration = 0.25;

	RotOscillation.Pitch.Amplitude = 0.8;
	RotOscillation.Pitch.Frequency = 18.0;

	RotOscillation.Roll.Amplitude = 0.6;
	RotOscillation.Roll.Frequency = 9.0;

	RotOscillation.Yaw.Amplitude = 0.4;
	RotOscillation.Yaw.Frequency = 0.7;

	bSingleInstance = true;
}