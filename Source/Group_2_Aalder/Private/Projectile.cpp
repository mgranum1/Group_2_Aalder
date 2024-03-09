// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Projectile.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    //Collision object and RootObject
    RootSphere = CreateDefaultSubobject<USphereComponent>(TEXT("BulletCollider"));
    RootComponent = RootSphere;
    RootSphere->SetGenerateOverlapEvents(true);
    RootSphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);

    ///Set up the visual component - the actual mesh is set in Blueprint
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
   /* MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);*/
    MeshComponent->SetupAttachment(RootComponent);


}

void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Warning, TEXT("Bullet Overlap %s"), *OtherActor->GetName());
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, FString::Printf(TEXT("%s"), *OtherActor->GetName()));

    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, TEXT("HIT"));
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    FVector NewLocation = GetActorLocation();
    NewLocation += GetActorForwardVector() * Speed * DeltaTime;
    SetActorLocation(NewLocation);

    TimeLived += DeltaTime;
    if (TimeLived > TimeBeforeDestroy)
    {
        this->Destroy();
    }


}






