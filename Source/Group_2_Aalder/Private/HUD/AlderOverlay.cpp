// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/AlderOverlay.h"
#include "Components/ProgressBar.h"

void UAlderOverlay::SetAmmoCooldownPercent(float Percent)
{
	if (AmmoCooldown) {
		
		AmmoCooldown->SetPercent(Percent);

	}
}
