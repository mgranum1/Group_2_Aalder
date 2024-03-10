// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AalderPlayerCharacter.generated.h"

class UInputMappingContext;
class UCameraComponent;
struct FInputActionValue;
class UInputComponent;
class UInputAction;
class USpringArmComponent;
class AProjectile;

UCLASS()
class GROUP_2_AALDER_API AAalderPlayerCharacter : public ACharacter
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

	//Projectile/Bullet reference
	UPROPERTY(EditAnywhere, Category = "Setup")
	TSubclassOf<AProjectile> BulletBlueprint;

	//spawn offset for bullet, init verdi 50.f kan endres i blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BulletSpawnOffset")
	float SpawnZOffset = 50.f;

public:
	//Gliding attributes
	FVector CurrentVelocity;
	float Delta;
	float minHeight = 300;

private:
	bool bIsGliding = false;
	bool OriginalOrientRotation;
	float OriginalGravityScale;
	float OriginalWalkingSpeed;
	float OriginalDecelration;
	float OriginalAcceleration;
	float OriginalAirControl;
	bool OriginalDesiredRotation;
	


	void EnableGliding();

	void StartGliding();

	void StopGliding();

	bool CanStartGliding();

	void RecordOriginalSettings();

	void DescendPlayer();

	void ApplyOriginalSettings();

	////////////////////////////////////////


	void Move(const FInputActionValue& Value);

	void LookAround(const FInputActionValue& Value);

	void Fire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*virtual void Tick(float DeltaSeconds)override;*/


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return SpringArm; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return TPCameraComponent; }

};
