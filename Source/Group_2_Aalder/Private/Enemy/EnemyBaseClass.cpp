// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyBaseClass.h"
#include "HUD/HealthBarComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

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


	/*Melee Components*/
	HandCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hand Collider"));
	HandCollider->SetupAttachment(GetRootComponent());


	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());

	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
	
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

		

		if (HealthBarWidget) {
			HealthPercent > 0 ? HealthBarWidget->SetPercentHealth(HealthPercent) : Dead();
			UE_LOG(LogTemp, Warning, TEXT("Enemy Health: %f"), Health);
		}
	}

	return 0.0f;
}

void AEnemyBaseClass::OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	FHitResult BoxHit;

	UKismetSystemLibrary::BoxTraceSingle(

		this,
		Start,
		End,
		FVector(5.f, 5.f, 5.f),
		BoxTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		BoxHit,
		true

	);

	// Log BoxHit information to console
	UE_LOG(LogTemp, Warning, TEXT("Box Hit Location: %s"), *BoxHit.ImpactPoint.ToString());

	if (BoxHit.GetActor() != nullptr)
	{
		// Log BoxHit information to console
		UE_LOG(LogTemp, Warning, TEXT("You hit: %s"), *BoxHit.GetActor()->GetName());

	}

	if (BoxHit.GetActor())
	{
		IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
		if (HitInterface)
		{
			HitInterface->GetHit(BoxHit.ImpactPoint);
		}

		UGameplayStatics::ApplyDamage(
			BoxHit.GetActor(),
			BaseDamageAmount,
			this->GetController(),
			this,
			UDamageType::StaticClass()

		);
	}


}

void AEnemyBaseClass::Dead()
{
	Destroy();
}

