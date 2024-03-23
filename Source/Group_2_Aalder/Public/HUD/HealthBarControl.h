// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarControl.generated.h"

/**
 * 
 */
UCLASS()
class GROUP_2_AALDER_API UHealthBarControl : public UUserWidget
{
	GENERATED_BODY()

public:

	//remember to mach BP_HealthBar name excactly
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	
};

