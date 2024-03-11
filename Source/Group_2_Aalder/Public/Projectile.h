// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class GROUP_2_AALDER_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();


	/**The collision shape of the bullet*/
	UPROPERTY(EditAnywhere, Category = "Bullet")
	class USphereComponent* CollisionSphere{ nullptr };

	// Mesh bullet
	UPROPERTY(VisibleAnywhere, Category = "Bullet")
	class UStaticMeshComponent* MeshComponent{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	class UProjectileMovementComponent* ProjectileMovement;

	// initital speed 5000, can change speed in blueprint
	UPROPERTY(EditAnywhere, Category = "Bullet")
	float Speed{ 5000.0f };

	/**Time before bullet selfdestruct*/
	UPROPERTY(EditAnywhere, Category = "Bullet")
	float TimeBeforeDestroy{ 5.f };

	float TimeLived{ 0 };

	/** called when something enters the sphere component */
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
