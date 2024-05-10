// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBaseClass.h"

#include "Interfaces/HitInterface.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"

#include "Sound/SoundCue.h"

#include "BossEnemy.generated.h"

class UAttribruteComponent;
class UHealthBarComponent;


/**
 * 
 */
UCLASS()
class GROUP_2_AALDER_API ABossEnemy : public AEnemyBaseClass
{
	GENERATED_BODY()

public:
	ABossEnemy();

	void Dead() override;


};
