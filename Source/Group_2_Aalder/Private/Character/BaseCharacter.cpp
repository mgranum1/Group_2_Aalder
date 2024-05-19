// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	Attributes = CreateDefaultSubobject<UAttribruteComponent>(TEXT("Attributes"));
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());


	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());

	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
}

void ABaseCharacter::GetHit(const FVector& ImpactPoint)
{

}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Attributes) {

		Attributes->SetHealth(DamageAmount);

		float Health = Attributes->GetHealth();
		float HealthPercent = Attributes->GetHealthPercent();

		UGameplayStatics::PlaySoundAtLocation(this, HitSoundCue, GetActorLocation());

		if (HealthBarWidget) {
			HealthPercent > 0 ? HealthBarWidget->SetPercentHealth(HealthPercent) : Dead();
			//UE_LOG(LogTemp, Warning, TEXT("Enemy Health: %f"), Health);
		}
	}

	return 0.0f;

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

