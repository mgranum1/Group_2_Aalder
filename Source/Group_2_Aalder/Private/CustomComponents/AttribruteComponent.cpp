// Fill out your copyright notice in the Description page of Project Settings.



#include "CustomComponents/AttribruteComponent.h"

// Sets default values for this component's properties
UAttribruteComponent::UAttribruteComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


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


float UAttribruteComponent::GetHealthPercent()
{
	return Health / MaxHealth;
}

float UAttribruteComponent::SetHealth(float Damage)
{
	if (Health > 0) {
		return Health -= Damage;
	}
	else  {
		
		return 0;//death
	}

}

float UAttribruteComponent::GetHealth()
{
	return Health;
}

float UAttribruteComponent::SetMaxHealth()
{
	return Health = MaxHealth;

}

