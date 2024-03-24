// Fill out your copyright notice in the Description page of Project Settings.
#include "Items/Projectile.h"
#include "Components/SphereComponent.h"
#include "Interfaces/HitInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Character/AalderPlayerCharacter.h"
#include "Components/StaticMeshComponent.h"



// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    //Collision object and RootObject
    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("BulletCollider"));
    CollisionSphere->InitSphereRadius(20.0f);

    RootComponent = CollisionSphere;

    ///Set up the visual component - the actual mesh is set in Blueprint
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));

    MeshComponent->SetupAttachment(RootComponent);


}

void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

    UE_LOG(LogTemp, Warning, TEXT("Bullet Overlap %s"), *OtherActor->GetName());
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, FString::Printf(TEXT("%s"), *OtherActor->GetClass()->GetName()));

    if (OtherActor->GetClass()->GetName() == "ActorTEST_C") {

        //take damage istedenfor Destroy, fikses senere
        OtherActor->Destroy();
    }

    FHitResult BoxHit;
    if (BoxHit.GetActor())
    {
        IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
        if (HitInterface)
        {
            HitInterface->GetHit(BoxHit.ImpactPoint);
        }

        UGameplayStatics::ApplyDamage(
            BoxHit.GetActor(),
            20.0f,
            GetInstigator()->GetController(),
            this,
            UDamageType::StaticClass()

        );
    }
}




// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
    CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
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






