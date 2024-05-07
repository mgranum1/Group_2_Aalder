// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "AlderOverlay.generated.h"

/**
 * 
 */
class UProgressBar;

UCLASS()
class GROUP_2_AALDER_API UAlderOverlay : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetAmmoCooldownPercent(float Percent);
	float GetAmmoCooldownPercent();

	UFUNCTION(BlueprintCallable)
	void SetHealthPercent(float Percent);

	float GetHealthPercent();


	UPROPERTY(meta = (BindWidget))
	UProgressBar* AmmoCooldown;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY()
	UTextBlock* Text;

};
