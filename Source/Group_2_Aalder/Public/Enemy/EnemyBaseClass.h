// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/HitInterface.h"
#include "GameFramework/Character.h"

#include "EnemyBaseClass.generated.h"

class UAttribruteComponent;
class UWidgetComponent;


UCLASS()
class GROUP_2_AALDER_API AEnemyBaseClass : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBaseClass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit(const FVector& ImpactPoint) override;

	UPROPERTY(VisibleAnywhere)
	UAttribruteComponent* Attributes;

	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* HealthBarWidget;

};
