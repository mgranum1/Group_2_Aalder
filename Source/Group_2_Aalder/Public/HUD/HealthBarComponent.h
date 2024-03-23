// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

class UHealthBarControl;
/**
 * 
 */
UCLASS()
class GROUP_2_AALDER_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:

	void SetPercentHealth(float Percent);

private:
	
	UPROPERTY()
	UHealthBarControl* HealthBarWidget;

};
