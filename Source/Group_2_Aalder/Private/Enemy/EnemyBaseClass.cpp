// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyBaseClass.h"
#include "HUD/HealthBarComponent.h"
#include "Engine/Engine.h"
#include "CustomComponents/AttribruteComponent.h"
#include "HUD/HealthBarComponent.h"

// Sets default values
AEnemyBaseClass::AEnemyBaseClass()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Attributes = CreateDefaultSubobject<UAttribruteComponent>(TEXT("Attributes"));
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
	
}

// Called when the game starts or when spawned
void AEnemyBaseClass::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AEnemyBaseClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyBaseClass::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyBaseClass::GetHit(const FVector& ImpactPoint)
{
#define DRAW_SPHERE_COLOR(Location, Color) DrawDebugSphere(GetWorld(), Location, 8.f, 12, Color, false, 5.f);
	DRAW_SPHERE_COLOR(ImpactPoint, FColor::Orange);
}

float AEnemyBaseClass::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	if (Attributes) {

		Attributes->SetHealth(DamageAmount);

		float Health = Attributes->GetHealth();
		float HealthPercent = Attributes->GetHealthPercent();

		UE_LOG(LogTemp, Warning, TEXT("Enemy Health: %f"), Health);

		if (HealthBarWidget) {
			HealthPercent > 0 ? HealthBarWidget->SetPercentHealth(HealthPercent) : Dead();

		}
	}

	return 0.0f;
}

void AEnemyBaseClass::Dead()
{
	Destroy();
}

