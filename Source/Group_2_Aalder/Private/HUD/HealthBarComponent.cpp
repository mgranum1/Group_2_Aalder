// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthBarComponent.h"
#include "HUD/HealthBarControl.h"
#include "Components/ProgressBar.h"

void UHealthBarComponent::SetPercentHealth(float Percent)
{
	if (HealthBarWidget == nullptr) {
		 
		HealthBarWidget = Cast<UHealthBarControl>(GetUserWidgetObject());
	
	}

	if (HealthBarWidget && HealthBarWidget->HealthBar) {

		HealthBarWidget->HealthBar->SetPercent(Percent);
	}

	
}
