// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBaseClass.h"
#include "Items/Projectile.h"
#include "Interfaces/HitInterface.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Items/SpawnKey.h"
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

	void SpawnKey();

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TSubclassOf<class ASpawnKey> SpawnBossKey;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FVector MuzzleOffset;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AProjectile> BulletBlueprint;

	UFUNCTION(BlueprintCallable)
	void Fire();

};
