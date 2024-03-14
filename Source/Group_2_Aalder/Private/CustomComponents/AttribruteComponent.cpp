// Fill out your copyright notice in the Description page of Project Settings.



#include "CustomComponents/AttribruteComponent.h"

// Sets default values for this component's properties
UAttribruteComponent::UAttribruteComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	MaxHealth = 100;
	Health = MaxHealth;

	// ...
}


// Called when the game starts
void UAttribruteComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAttribruteComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAttribruteComponent::ReceiveDamage(float damage)
{
	if (Health > 0) {
		Health -= damage;
	}
	else if (Health == 0) {
		//death
	}
	GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Black, FString::Printf(TEXT("Current health: %f"),Health));
}

float UAttribruteComponent::GetHealthPercent()
{
	return 0.0f;
}
