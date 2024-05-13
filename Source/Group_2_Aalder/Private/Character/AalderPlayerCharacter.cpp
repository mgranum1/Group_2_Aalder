// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AalderPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "Engine/PostProcessVolume.h"
#include "Math/UnrealMathUtility.h"
#include "Interfaces/HitInterface.h"
#include "CustomComponents/AttribruteComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Components/WidgetComponent.h"
#include "EngineUtils.h"
#include "HUD/Alder_HUD.h"
#include "HUD/AlderOverlay.h"
#include "Animation/AnimMontage.h"
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

	USkeletalMeshComponent* MeshComponent = GetMesh();

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

	/*Melee Components*/
	BeakCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Beak Collider"));
	BeakCollider->SetupAttachment(GetRootComponent());
	BeakCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());

	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());

	bIsDead = false;
	bIsInFirstPerson = false;
	bCanMeleeAttack = true;
	NumOfKeys = 0;
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
	
	BeakCollider->OnComponentBeginOverlap.AddDynamic(this, &AAalderPlayerCharacter::OnBoxOverlap);
	InitializeAlderOverlay();
	
	
}



void AAalderPlayerCharacter::Dead()
{
	
}

void AAalderPlayerCharacter::InitializeAlderOverlay()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController) {

		AAlder_HUD* AlderHUD = Cast<AAlder_HUD>(PlayerController->GetHUD());

		if (AlderHUD) {


			AlderOverlay = AlderHUD->GetAlderOverlay();
			AlderOverlay->Key1->SetOpacity(0);
			AlderOverlay->Key2->SetOpacity(0);

		}
	}

}

void AAalderPlayerCharacter::GetHit(const FVector& ImpactPoint)
{
#define DRAW_SPHERE_COLOR(Location, Color) DrawDebugSphere(GetWorld(), Location, 8.f, 12, Color, false, 5.f);
	DRAW_SPHERE_COLOR(ImpactPoint, FColor::Orange);

}

void AAalderPlayerCharacter::OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	FHitResult BoxHit;

	UKismetSystemLibrary::BoxTraceSingle(

		this,
		Start,
		End,
		FVector(5.f, 5.f, 5.f),
		BoxTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		BoxHit,
		true
		
	);

	// Log BoxHit information to console
	UE_LOG(LogTemp, Warning, TEXT("Box Hit Location: %s"), *BoxHit.ImpactPoint.ToString());

	if (BoxHit.GetActor() != nullptr)
	{
		// Log BoxHit information to console
		UE_LOG(LogTemp, Warning, TEXT("You hit: %s"), *BoxHit.GetActor()->GetName());

	}

	if (BoxHit.GetActor() && bCanMeleeAttack)
	{
		IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
		if (HitInterface)
		{
			HitInterface->GetHit(BoxHit.ImpactPoint);
		}

		UGameplayStatics::ApplyDamage(
			BoxHit.GetActor(),
			BaseDamageAmount,
			this->GetController(),
			this,
			UDamageType::StaticClass()

		);
	}



}

float AAalderPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
		if (Attributes) {

		Attributes->SetHealth(DamageAmount);

		float Health = Attributes->GetHealth();
		float HealthPercent = Attributes->GetHealthPercent();

		if (AlderOverlay) {
			AlderOverlay->SetHealthPercent(HealthPercent);
		}

		}


	return 0.0f;
}

void AAalderPlayerCharacter::HealUpMaxHealth()
{
	if (Attributes) {
		Attributes->SetMaxHealth();
	}
	if (AlderOverlay) {
		AlderOverlay->SetHealthPercent(1.0f);
	}

}

bool AAalderPlayerCharacter::GetIsInFirstPerson()
{
	return bIsInFirstPerson;
}

bool AAalderPlayerCharacter::GetCanShowLowHealthWidget()
{
	return bCanShowLowHealthWidget;
}

void AAalderPlayerCharacter::ShowLowHealthWidget(UWidgetComponent* Widget)
{
	if (bCanShowLowHealthWidget) {
		Widget->SetVisibility(true);
	}
}

int AAalderPlayerCharacter::SetNumOfKeys(int NumOfKeysToAdd)
{
	return NumOfKeys = NumOfKeys + NumOfKeysToAdd;
}

int AAalderPlayerCharacter::GetNumOfKeys()
{
	return NumOfKeys;
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


void AAalderPlayerCharacter::Fire()
{
	
	// Get the camera transform.
	FVector CameraLocation;
	FRotator CameraRotation;
	GetActorEyesViewPoint(CameraLocation, CameraRotation);

	// Set MuzzleOffset to spawn projectiles slightly in front of the camera.
	MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

	// Transform MuzzleOffset from camera space to world space.
	FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

	// Skew the aim to be slightly upwards.
	FRotator MuzzleRotation = CameraRotation;
	MuzzleRotation.Pitch += 10.0f;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	if (bCanShoot) {
		
		GetWorld()->SpawnActor<AProjectile>(BulletBlueprint, MuzzleLocation, MuzzleRotation, SpawnParams);
		

		bCanShoot = false;
		GetWorldTimerManager().SetTimer(FireRateHandler, this, &AAalderPlayerCharacter::ResetFire, FireRate, false);
	}

	bIsShooting = true;

	/*Attributes->SetMaxHealth();
	AlderOverlay->SetHealthPercent(1.f);*/


}



void AAalderPlayerCharacter::ResetFire()
{
	bCanShoot = true;
}

void AAalderPlayerCharacter::ResetMelee()
{
	bCanMeleeAttack = true;
}


void AAalderPlayerCharacter::MeleeAttack()
{
	if (bCanMeleeAttack) {
		
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		AnimInstance->Montage_Play(AttackMontage);

		BeakCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);


		BeakCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		bCanMeleeAttack = false;
		GetWorldTimerManager().SetTimer(MeleeAttackTimerHandler, this, &AAalderPlayerCharacter::ResetMelee, MeleeAttackRate, false);

	}
	
	
}

void AAalderPlayerCharacter::ChangeCamView()
{

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	TArray<APostProcessVolume*> PostProcessVolumes;

	for (TActorIterator<APostProcessVolume> ActorItr(GetWorld()); ActorItr; ++ActorItr) {

		APostProcessVolume* PPUIter = *ActorItr;

		PostProcessVolumes.Add(PPUIter);

	}

	if (!bIsInFirstPerson) {
		
		PostProcessVolumes[1]->bEnabled = false;
		PostProcessVolumes[0]->bEnabled = true;
		bIsInFirstPerson = true;
	}

	else 
	{
		PostProcessVolumes[1]->bEnabled = true;
		PostProcessVolumes[0]->bEnabled = false;
		bIsInFirstPerson = false;
	
	}
	
}

void AAalderPlayerCharacter::ChangeZoomMode()
{
	if (bIsInFirstPerson && (SpringArm->TargetArmLength == CamZoomModes[0])) {
		for (int i = 0; i < 1; i++)
		{
			SpringArm->TargetArmLength = CamZoomModes[i + 1];
		}
		
	}
	


}

void AAalderPlayerCharacter::Pause()
{
}


// Called every frame
void AAalderPlayerCharacter::Tick(float DeltaSeconds)
{
	Delta = DeltaSeconds;
	DescendPlayer();

	if (bIsShooting) {

	

		float LerpValueChangeSpeed = 4.0f;
		
		if (AlderOverlay) {

			float LerpVal = FMath::FInterpTo(AlderOverlay->GetAmmoCooldownPercent(), 0, DeltaSeconds, LerpValueChangeSpeed);
			float LerpedValue = FMath::Lerp(FireRate, 0.0f, TimeElapsedAfterShot * LerpValueChangeSpeed);
					AlderOverlay->SetAmmoCooldownPercent(LerpVal);
					// lerp between 1 and 0
					TimeElapsedAfterShot += DeltaSeconds;

					
		}
		if (AlderOverlay->GetAmmoCooldownPercent() <= 0.0f && TimeElapsedAfterShot > 0 || TimeElapsedAfterShot >= FireRate) {

					AlderOverlay->SetAmmoCooldownPercent(1.f);

					bIsShooting = false;

					TimeElapsedAfterShot = 0;
		}
		
	
	}

	if (Attributes->GetHealthPercent() <= 0.3f && !bIsInFirstPerson) {
		
		bCanShowLowHealthWidget = true; 
		
		AlderOverlay->LowHealtMsg->SetOpacity(1);
	}
	else {
		AlderOverlay->LowHealtMsg->SetOpacity(0);
	}
	
	for (int i = 0; i < 2; i++)
	{
		if (NumOfKeys == 1) {
			AlderOverlay->Key1->SetOpacity(1);
		}

		else if (NumOfKeys == 2) {
			AlderOverlay->Key2->SetOpacity(1);
		}

	}

	if (Attributes->GetHealth() <= 0.0f && bIsDead)
	{
		bIsDead = true;
		Destroy();
	

	}




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
		EnhancedInputComponent->BindAction(CamSwitchAction, ETriggerEvent::Completed, this, &AAalderPlayerCharacter::ChangeCamView);
		EnhancedInputComponent->BindAction(ZoomSwitchAction, ETriggerEvent::Completed, this, &AAalderPlayerCharacter::ChangeZoomMode);

		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &AAalderPlayerCharacter::Pause);
	}

}

