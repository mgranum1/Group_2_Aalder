// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/SpawnKey.h"

// Sets default values
ASpawnKey::ASpawnKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Trigger"));
	SphereTrigger->SetupAttachment(GetRootComponent());


}

// Called when the game starts or when spawned
void ASpawnKey::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

