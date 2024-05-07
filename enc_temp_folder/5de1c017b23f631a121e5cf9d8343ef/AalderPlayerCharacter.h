// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/BoxComponent.h"
#include "CustomComponents/AttribruteComponent.h"
#include "Interfaces/HitInterface.h"
#include "GameFramework/Character.h"
#include "HUD/AlderOverlay.h"
#include "AalderPlayerCharacter.generated.h"

class UInputMappingContext;
class UCameraComponent;
struct FInputActionValue;
class UInputComponent;
class UAnimMontage;
class UInputAction;
class USpringArmComponent;
class AProjectile;

UCLASS()
class GROUP_2_AALDER_API AAalderPlayerCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAalderPlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	////Komponenter

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* TPCameraComponent;

	UPROPERTY(EditAnywhere, Category = "Camera")
	UCameraComponent* FPCamera;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Camera")
	USpringArmComponent* SpringArm;

	////Input kontroller

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Input")
	UInputMappingContext* IMC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* GlidingAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float DescendingRate = 300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MeleeAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* CamSwitchAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* ZoomSwitchAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* PauseAction;

	
	//// ______   COMBAT  ____________

	////Projectile/skyting 
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AProjectile> BulletBlueprint;
	
	//brukes denne?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float SpawnZOffset = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float FireRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bCanShoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FVector MuzzleOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float BaseDamageAmount = 10.0f;

	FTimerHandle FireRateHandler;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BeakCollider;

	void InitializeAlderOverlay();
	
	UPROPERTY(VisibleAnywhere)
	UAlderOverlay* AlderOverlay;


	virtual void GetHit(const FVector& ImpactPoint) override;


	UFUNCTION()
	void OnBoxOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//// Attributes og diverse ________
	UPROPERTY(VisibleAnywhere)
	UAttribruteComponent* Attributes;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	void HealUpMaxHealth();

public:
	//Gliding attributes
	FVector CurrentVelocity;
	float Delta;
	float minHeight = 300;
	float TimeElapsedAfterShot;
	bool bIsShooting;
	

private:
	bool bIsGliding = false;
	bool OriginalOrientRotation;
	float OriginalGravityScale;
	float OriginalWalkingSpeed;
	float OriginalDecelration;
	float OriginalAcceleration;
	float OriginalAirControl;
	bool OriginalDesiredRotation;

	///Animation montages

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;

	///Cam variabler

	UPROPERTY(VisibleAnywhere, Category="Custom Cam Settings")
	bool bIsInFirstPerson;

	UPROPERTY(EditAnywhere, Category = "Custom Cam Settings")
	TArray<int> CamZoomModes;

	UPROPERTY(EditAnywhere, Category = "Custom Cam Settings")
	int ZoomMode01;

	UPROPERTY(EditAnywhere, Category = "Custom Cam Settings")
	int ZoomMode02;

	UPROPERTY(EditAnywhere, Category = "Custom Cam Settings")
	int ZoomMode03;

	////Funksjoner


	void EnableGliding();

	void StartGliding();

	void StopGliding();

	bool CanStartGliding();

	void RecordOriginalSettings();

	void DescendPlayer();

	void ApplyOriginalSettings();

	void Move(const FInputActionValue& Value);

	void LookAround(const FInputActionValue& Value);

	void Fire();

	void ResetFire();

	void MeleeAttack();

	void ChangeCamView();

	void ChangeZoomMode();

	void Pause();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*virtual void Tick(float DeltaSeconds)override;*/


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return SpringArm; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return TPCameraComponent; }



private:


};
