// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttribruteComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GROUP_2_AALDER_API UAttribruteComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttribruteComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	float GetHealthPercent();

	UFUNCTION(BlueprintCallable)
	float SetHealth(float Damage);

	UFUNCTION(BlueprintCallable)
	float GetHealth();

	UFUNCTION(BlueprintCallable)
	float SetMaxHealth();

	UFUNCTION(BlueprintCallable)
	float GetDamageAmount();


private:

	//Justeres i blueprint for hver asset
	UPROPERTY(EditAnywhere, Category ="Actor Attributes")
	float Health;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float DamageAmount = 10.0f;

};
