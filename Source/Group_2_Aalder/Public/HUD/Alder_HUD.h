// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Alder_HUD.generated.h"

/**
 * 
 */
class UAlderOverlay;

UCLASS()
class GROUP_2_AALDER_API AAlder_HUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
		UPROPERTY()
		UAlderOverlay* AlderOverlay;

public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAlderOverlay> AlderOverlayClass;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UAlderOverlay* GetAlderOverlay() const { return AlderOverlay; }
	
};
