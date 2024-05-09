// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyBaseClass.h"
#include "HUD/HealthBarComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Character/AalderPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
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
	
	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());

	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
	
}

// Called when the game starts or when spawned
void AEnemyBaseClass::BeginPlay()
{
	Super::BeginPlay();

	HandCollider->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBaseClass::OnComponentHit);
	HandCollider->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("hand_rSocket"));
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

		UGameplayStatics::PlaySoundAtLocation(this, HitSoundCue, GetActorLocation());
		
		if (HealthBarWidget) {
			HealthPercent > 0 ? HealthBarWidget->SetPercentHealth(HealthPercent) : Dead();
			UE_LOG(LogTemp, Warning, TEXT("Enemy Health: %f"), Health);
		}
	}

	return 0.0f;
}

void AEnemyBaseClass::OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	UE_LOG(LogTemp, Warning, TEXT("Enemy Hit: %s"), *OtherActor->GetName());

	AAalderPlayerCharacter* Player = Cast<AAalderPlayerCharacter>(OtherActor);

	if (OtherActor == Player)
	{
		IHitInterface* HitInterface = Cast<IHitInterface>(OtherActor);
		if (HitInterface && OtherActor != this)
		{
			UGameplayStatics::ApplyDamage(
			OtherActor,
			BaseDamageAmount,
			this->GetController(),
			this,
			UDamageType::StaticClass()
			);

		}

		

	
	}

}

void AEnemyBaseClass::OnComponentHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy Hit: %s"), *OtherActor->GetName());

	if (OtherActor && OtherActor != this)
	{
		IHitInterface* HitInterface = Cast<IHitInterface>(OtherActor);
		if (HitInterface)
		{
			UGameplayStatics::ApplyDamage(
				OtherActor,
				BaseDamageAmount,
				this->GetController(),
				this,
				UDamageType::StaticClass()
			);

		}




	}
}

void AEnemyBaseClass::Attack(float DamageAmount)
{
	
	GEngine->AddOnScreenDebugMessage(1, 0.5f, FColor::Blue, TEXT("Attacking"));

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	AnimInstance->Montage_Play(AttackMontage);

}

void AEnemyBaseClass::Dead()
{
	Destroy();
}

void AEnemyBaseClass::SetHandCollision(ECollisionEnabled::Type CollisionEnabled)
{
	HandCollider->SetCollisionEnabled(CollisionEnabled);

}

