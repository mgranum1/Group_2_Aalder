// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Alder_HUD.h"
#include "HUD/AlderOverlay.h"

void AAlder_HUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && AlderOverlayClass)
		{
			AlderOverlay = CreateWidget<UAlderOverlay>(Controller, AlderOverlayClass);
			AlderOverlay->AddToViewport();
		}
	}
}
