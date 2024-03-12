// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat.h"

// Sets default values
ACombat::ACombat()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	health = maxHealth;

}

// Called when the game starts or when spawned
void ACombat::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACombat::TakeDamage(float DamageAmount, float memberHealth, bool isDead)
{
	isDead = false;

	if (memberHealth > 0) {
		memberHealth = memberHealth - DamageAmount;
	}
	else
	{
		isDead = true;
	}
	
	GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Blue, FString::Printf(TEXT("Current health: %f"), memberHealth));


}

// Called every frame
void ACombat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

