// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AalderPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"

#include "CustomComponents/AttribruteComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "TimerManager.h"
#include "Items/Projectile.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"


// Sets default values
AAalderPlayerCharacter::AAalderPlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Attributes
	Attributes = CreateDefaultSubobject<UAttribruteComponent>(TEXT("Attributes"));



	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Kan ogs� endres i Character blueprint
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	//Spring arm component

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	SpringArm->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	/*Camera Component*/
	TPCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	TPCameraComponent->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	TPCameraComponent->bUsePawnControlRotation = false;


	SetupStimulusSource();

}

//AI thingy
void AAalderPlayerCharacter::SetupStimulusSource()
{
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	if(StimulusSource)
	{
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimulusSource->RegisterWithPerceptionSystem();

	}
}

float AAalderPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return 0.0f;
}

//Gliding function
void AAalderPlayerCharacter::EnableGliding()
{
	if (bIsGliding == false)
	{
		StartGliding();
	}

	else 
	{
		StopGliding();
	}

}

void AAalderPlayerCharacter::StartGliding()
{
	if (CanStartGliding())
	{
		CurrentVelocity = GetCharacterMovement()->Velocity;
		bIsGliding = true;
		RecordOriginalSettings();



		GetCharacterMovement()->RotationRate = FRotator(0.f, 250.f, 0.f);

		GetCharacterMovement()->GravityScale = 0.0;
		GetCharacterMovement()->AirControl = 0.9;
		GetCharacterMovement()->BrakingDecelerationFalling = 350.f;
		GetCharacterMovement()->MaxAcceleration = 1024;
		GetCharacterMovement()->MaxWalkSpeed = 600;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
	}
}

void AAalderPlayerCharacter::StopGliding()
{
	ApplyOriginalSettings();
	bIsGliding = false;
}

bool AAalderPlayerCharacter::CanStartGliding()
{
	FHitResult Hit;
	FVector TraceStart = GetActorLocation();
	FVector TraceEnd = GetActorLocation() + GetActorUpVector() * minHeight * -1.f;

	FCollisionQueryParams QueryParams;

	QueryParams.AddIgnoredActor(this);

	TEnumAsByte<ECollisionChannel> TraceProperties = ECC_Visibility;

	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, TraceProperties, QueryParams);

	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, Hit.bBlockingHit ? FColor::Red : FColor::Yellow);

	if (Hit.bBlockingHit == false && GetCharacterMovement()->IsFalling() == true)
	{
		return true;
	}

	else
	{
		return false;
	}

}

void AAalderPlayerCharacter::RecordOriginalSettings()
{
	
	OriginalOrientRotation = GetCharacterMovement()->bOrientRotationToMovement;
	OriginalGravityScale = GetCharacterMovement()->GravityScale;
	OriginalAirControl = GetCharacterMovement()->AirControl;
	OriginalDecelration = GetCharacterMovement()->BrakingDecelerationFalling;
	OriginalAcceleration = GetCharacterMovement()->MaxAcceleration;
	OriginalWalkingSpeed = GetCharacterMovement()->MaxWalkSpeed;
	OriginalDesiredRotation = GetCharacterMovement()->bUseControllerDesiredRotation;


	bCanShoot = true;

}

void AAalderPlayerCharacter::DescendPlayer()
{
	if (CurrentVelocity.Z != DescendingRate * -1.f && bIsGliding == true)
	{
		CurrentVelocity.Z = UKismetMathLibrary::FInterpTo(CurrentVelocity.Z, DescendingRate, Delta, 3.f);
		GetCharacterMovement()->Velocity.Z = DescendingRate * -1.f;
	}
}

void AAalderPlayerCharacter::ApplyOriginalSettings()
{
	GetCharacterMovement()->bOrientRotationToMovement = OriginalOrientRotation;
	GetCharacterMovement()->GravityScale = OriginalGravityScale;
	GetCharacterMovement()->AirControl = OriginalAirControl;
	GetCharacterMovement()->BrakingDecelerationFalling = OriginalDecelration;
	GetCharacterMovement()->MaxAcceleration = OriginalAcceleration;
	GetCharacterMovement()->MaxWalkSpeed = OriginalWalkingSpeed;
	GetCharacterMovement()->bUseControllerDesiredRotation = OriginalDesiredRotation;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
}

void AAalderPlayerCharacter::Move(const FInputActionValue& Value)
{
	/*GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Triggering the move function"));*/

	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AAalderPlayerCharacter::LookAround(const FInputActionValue& Value)
{
	FVector2D LookAroundVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAroundVector.X);
		AddControllerPitchInput(LookAroundVector.Y);
	}
}



// Called when the game starts or when spawned
void AAalderPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Adding the Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC, 0);
		}
	}

}


void AAalderPlayerCharacter::Fire()
{
	/*GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, TEXT("Firing"));*/
	
	/*CombatComponent->TakeDamage(10.0f, PlayerHealth, bPlayerIsDead);*/

	if (bCanShoot) {
		GetWorld()->SpawnActor<AProjectile>(BulletBlueprint, GetActorLocation() +
			GetActorForwardVector() * 100.f + FVector(0.f, 0.f, SpawnZOffset), GetActorRotation());

		bCanShoot = false;
		GetWorldTimerManager().SetTimer(FireRateHandler, this, &AAalderPlayerCharacter::ResetFire, FireRate, false);
	}
		
}



void AAalderPlayerCharacter::ResetFire()
{
	bCanShoot = true;
}


void AAalderPlayerCharacter::MeleeAttack()
{
	/*GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, TEXT("Firing"));*/

	if (Attributes) {
		Attributes->ReceiveDamage(10.0f);
	}

}


// Called every frame
void AAalderPlayerCharacter::Tick(float DeltaSeconds)
{
	Delta = DeltaSeconds;
	DescendPlayer();
}

// Called to bind functionality to input
void AAalderPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAalderPlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAalderPlayerCharacter::LookAround);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(GlidingAction, ETriggerEvent::Started, this, &AAalderPlayerCharacter::EnableGliding);

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AAalderPlayerCharacter::Fire);

		EnhancedInputComponent->BindAction(MeleeAction, ETriggerEvent::Completed, this, &AAalderPlayerCharacter::MeleeAttack);
	}

}
