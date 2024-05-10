// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/BossEnemy.h"
#include "HUD/HealthBarComponent.h"
#include "Character/AalderPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "CustomComponents/AttribruteComponent.h"


ABossEnemy::ABossEnemy()
{

	PrimaryActorTick.bCanEverTick = true;

	


}

void ABossEnemy::Dead()
{
	GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Black, TEXT("Dead"));
	Destroy();
}


