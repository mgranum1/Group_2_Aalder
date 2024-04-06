// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/AlderOverlay.h"
#include "Components/ProgressBar.h"

void UAlderOverlay::SetAmmoCooldownPercent(float Percent)
{
	if (AmmoCooldown) {
		
		AmmoCooldown->SetPercent(Percent);

	}
}

float UAlderOverlay::GetAmmoCooldownPercent()
{
	return AmmoCooldown->GetPercent();
}

void UAlderOverlay::SetHealthPercent(float Percent)
{
	if (HealthBar) {
		HealthBar->SetPercent(Percent);
	}
}

float UAlderOverlay::GetHealthPercent()
{
	return HealthBar->GetPercent();
}
