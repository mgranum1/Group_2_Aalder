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
	SphereTrigger->OnComponentBeginOverlap.AddDynamic(this, &ASpawnKey::OnOverlapBegin);
}

// Called every frame
void ASpawnKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnKey::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor) {

		PlayerCharacter = Cast<AAalderPlayerCharacter>(OtherActor);

		if (PlayerCharacter && OtherActor->ActorHasTag("Player")) {
			
			PlayerCharacter->SetNumOfKeys(1);
			GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Black, FString::FromInt(PlayerCharacter->GetNumOfKeys()));
			Destroy();

		}
	}
}

