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
	//GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Black, TEXT("Dead"));
	SpawnKey();
	Destroy();
	
}

void ABossEnemy::SpawnKey()
{
	UWorld* World = GetWorld();
	if (World)
	{
		ASpawnKey* SpawnedKey = World->SpawnActor<ASpawnKey>(SpawnBossKey, GetActorLocation(), GetActorRotation());
		
	}
}

void ABossEnemy::Fire()
{

	// Get the camera transform.
	FVector CameraLocation;
	FRotator CameraRotation;
	GetActorEyesViewPoint(CameraLocation, CameraRotation);

	// Set MuzzleOffset to spawn projectiles slightly in front of the camera.
	MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

	// Transform MuzzleOffset from camera space to world space.
	FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

	// Skew the aim to be slightly upwards.
	FRotator MuzzleRotation = CameraRotation;
	

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	{

	GetWorld()->SpawnActor<AProjectile>(BulletBlueprint, MuzzleLocation, MuzzleRotation, SpawnParams);


		
		
	}

	

	/*Attributes->SetMaxHealth();
	AlderOverlay->SetHealthPercent(1.f);*/
}


