// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PyramidMathCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "ExplorerMovementComponent.h"
#include "TimerManager.h"
#include "Shootable.h"
#include "ShootablePhysicalMaterial.h"
#include "PlayerInteractable.h"
#include "TorchActor.h"
#include "DrawDebugHelpers.h"
#include "ExplorerHUD.h"

APyramidMathCharacter::APyramidMathCharacter(const FObjectInitializer& ObjInitializer) : Super(ObjInitializer.SetDefaultSubobjectClass<UExplorerMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	Gun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
	Gun->SetupAttachment(GetMesh());
	Gun->SetCollisionProfileName(FName("NoCollision"));

	GunAttachBone = FName("Item_R");

	MuzzleFlash = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MuzzleFlash"));
	MuzzleFlash->SetupAttachment(Gun);
	MuzzleFlash->bAutoActivate = false;

	GunFireSound = CreateDefaultSubobject<UAudioComponent>(TEXT("GunFireSound"));
	GunFireSound->SetupAttachment(Gun);
	GunFireSound->bAutoActivate = false;

	GunDrySound = CreateDefaultSubobject<UAudioComponent>(TEXT("GunDrySound"));
	GunDrySound->SetupAttachment(Gun);
	GunDrySound->bAutoActivate = false;

	Torch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TorchMesh"));
	Torch->SetupAttachment(GetMesh());
	Torch->SetCollisionProfileName(FName("NoCollision"));

	TorchAttachBone = FName("Item_L");

	TorchIntensity = 4000.0F;

	TorchFlame = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TorchFlame"));
	TorchFlame->SetupAttachment(Torch);
	TorchFlame->bAutoActivate = false;

	TorchLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("TorchLight"));
	TorchLight->SetupAttachment(Torch);
	TorchLight->SetIntensity(0.0F);

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = false; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	AmmoCapacity = 10;
	ShootingRate = 0.75F;
	bInfiniteBullets = false;

	TorchFuelCapacity = 100.0F;
	TorchFuelDrainRate = 1.0F;

	MaxSanity = 100.0F;
	SanityRecoverRate = 10.0F;
	SanityDrainRate = 1.0F;

	MaxHealth = 5;

	CurrentAmmo = 0;
	bWantsToShoot = false;
	bCanShoot = true;
	bIsPebbleLoaded = false;

	CurrentTorchFuel = TorchFuelCapacity;
	bIsTorchIgnited = false;

	InteractableActor = nullptr;

	CurrentSanity = MaxSanity;

	CurrentHealth = MaxHealth;
}

void APyramidMathCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC && InputEnabled())
	{
		FHitResult Hit;
		if (PC->GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, false, Hit))
		{
			if (Hit.bBlockingHit)
			{
				LookPoint = Hit.Location;
				FVector CharacterLocation = GetActorLocation();
				FVector LookDirection = LookPoint - CharacterLocation;
				LookDirection.Z = 0.0F;
				LookDirection.Normalize();
				FRotator NewRotation = LookDirection.Rotation();
				PC->SetControlRotation(FMath::Lerp<float>(PC->GetControlRotation(), NewRotation, 8.0F * DeltaSeconds));
			}
		}
	}
	if (bIsTorchIgnited)
	{
		CurrentTorchFuel -= TorchFuelDrainRate * DeltaSeconds;
		if (CurrentTorchFuel <= 0.0F)
		{
			CurrentTorchFuel = 0.0F;
			TorchFlame->Deactivate();
			bIsTorchIgnited = false;
		}
		float NewTorchIntensity = (CurrentTorchFuel / TorchFuelCapacity) * TorchIntensity;
		TorchLight->SetIntensity(NewTorchIntensity);
	}
	/*if (bIsTorchIgnited || HasActiveTorchesAround())
	{
		CurrentSanity = FMath::Clamp<float>(CurrentSanity + SanityRecoverRate * DeltaSeconds, 0.0F, MaxSanity);
	}
	else
	{
		CurrentSanity -= SanityDrainRate * DeltaSeconds;
		if (CurrentSanity <= 0.0F)
		{
			CurrentSanity = 0.0F;
			// Announce player's death!
		}
	}*/
}

void APyramidMathCharacter::MoveForward(float Value)
{
	if ((FollowCamera != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = FollowCamera->GetComponentRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APyramidMathCharacter::MoveRight(float Value)
{
	if ( (FollowCamera != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = FollowCamera->GetComponentRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void APyramidMathCharacter::StartShooting()
{
	bWantsToShoot = true;
	if (bCanShoot)
	{
		Shoot();
	}
}

void APyramidMathCharacter::StopShooting()
{
	bWantsToShoot = false;
}

void APyramidMathCharacter::Interact()
{
	if (InteractableActor)
	{
		IPlayerInteractable* Interactable = Cast<IPlayerInteractable>(InteractableActor);
		if (Interactable->CanInteract(this))
		{
			Interactable->Interact(this);
		}
	}
}

void APyramidMathCharacter::LoadPebble()
{
	bIsPebbleLoaded = true;
}

bool APyramidMathCharacter::HasPebbleLoaded() const
{
	return bIsPebbleLoaded;
}

void APyramidMathCharacter::AddAmmo(int32 InAmount)
{
	int32 OldAmmo = CurrentAmmo;
	CurrentAmmo = FMath::Clamp<int32>(CurrentAmmo + InAmount, 0, AmmoCapacity);
	OnAmmoChanged.Broadcast(CurrentAmmo, OldAmmo);
}

int32 APyramidMathCharacter::GetAmmo()
{
	return CurrentAmmo;
}

int32 APyramidMathCharacter::GetAmmoCapacity()
{
	return AmmoCapacity;
}

void APyramidMathCharacter::ToggleTorch()
{
	bIsTorchIgnited = !bIsTorchIgnited;
	if (bIsTorchIgnited)
	{
		TorchFlame->Activate(true);
		TorchLight->SetIntensity((CurrentTorchFuel / TorchFuelCapacity) * TorchIntensity);
	}
	else
	{
		TorchFlame->Deactivate();
		TorchLight->SetIntensity(0.0F);
	}
}

void APyramidMathCharacter::AddTorchFuel(float InAmount)
{
	CurrentTorchFuel = FMath::Clamp<float>(CurrentTorchFuel + InAmount, 0.0F, TorchFuelCapacity);
}

void APyramidMathCharacter::RemoveTorchFuel(float InAmount)
{
	if (InAmount < 0.0F || InAmount > CurrentTorchFuel)
	{
		return;
	}
	CurrentTorchFuel -= InAmount;
}

float APyramidMathCharacter::GetTorchFuelAmount()
{
	return CurrentTorchFuel;
}

float APyramidMathCharacter::GetTorchFuelCapacity()
{
	return TorchFuelCapacity;
}

float APyramidMathCharacter::GetSanity()
{
	return CurrentSanity;
}

bool APyramidMathCharacter::HasActiveTorchesAround()
{
	for (int i = 0; i < ActiveTorchActors.Num(); i++)
	{
		if(ActiveTorchActors[i]->IsIgnited())
		{
			return true;
		}
	}
	return false;
}

int32 APyramidMathCharacter::GetGold()
{
	return CurrentGold;
}

void APyramidMathCharacter::AddGold(int32 InAmount)
{
	if (InAmount <= 0)
	{
		return;
	}
	int32 OldGold = CurrentGold;
	CurrentGold += InAmount;
	OnGoldChanged.Broadcast(CurrentGold, OldGold);
}

void APyramidMathCharacter::RemoveGold(int32 InAmount)
{
	if (InAmount <= 0)
	{
		return;
	}
	int32 OldGold = CurrentGold;
	CurrentGold -= InAmount;
	OnGoldChanged.Broadcast(CurrentGold, OldGold);
}

int32 APyramidMathCharacter::GetHealth()
{
	return CurrentHealth;
}

int32 APyramidMathCharacter::GetMaxHealth()
{
	return MaxHealth;
}

void APyramidMathCharacter::AddHealth(int32 InAmount)
{
	if (InAmount <= 0)
	{
		return;
	}
	int32 OldHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp<int32>(CurrentHealth + InAmount, 0, MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth, OldHealth);
}

void APyramidMathCharacter::DealDamage(int32 InAmount, FVector DamageDirection)
{
	if (InAmount <= 0 || CurrentHealth == 0)
	{
		return;
	}
	int32 OldHealth = CurrentHealth;
	CurrentHealth -= InAmount;
	OnDamaged();
	if (CurrentHealth <= 0)
	{
		CurrentHealth = 0;
		Die(false);
	}
	OnHealthChanged.Broadcast(CurrentHealth, OldHealth);
	FVector LaunchVelocity = DamageDirection + FVector::UpVector;
	LaunchVelocity.Normalize();
	LaunchCharacter(LaunchVelocity * 500.0F, true, true);
}

bool APyramidMathCharacter::IsDead()
{
	return CurrentHealth == 0;
}

bool APyramidMathCharacter::NeedsRestocking()
{
	return CurrentAmmo < AmmoCapacity || CurrentTorchFuel < TorchFuelCapacity || CurrentHealth < MaxHealth;
}

void APyramidMathCharacter::BeginPlay()
{
	Super::BeginPlay();

	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, GunAttachBone);
	Torch->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TorchAttachBone);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APyramidMathCharacter::CharacterBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &APyramidMathCharacter::CharacterEndOverlap);
}

void APyramidMathCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &APyramidMathCharacter::StartShooting);
	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &APyramidMathCharacter::StopShooting);

	PlayerInputComponent->BindAction("Torch", IE_Pressed, this, &APyramidMathCharacter::ToggleTorch);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APyramidMathCharacter::Interact);

	PlayerInputComponent->BindAxis("MoveForward", this, &APyramidMathCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APyramidMathCharacter::MoveRight);
}

void APyramidMathCharacter::ShotReady()
{
	bCanShoot = true;
	if (bWantsToShoot)
	{
		Shoot();
	}
}

void APyramidMathCharacter::Shoot()
{
	if (bWantsToShoot && bCanShoot)
	{
		if (CurrentAmmo > 0 || bInfiniteBullets)
		{
			int32 OldAmmo = CurrentAmmo;
			CurrentAmmo = FMath::Clamp<int32>(CurrentAmmo - 1, 0, AmmoCapacity);
			OnAmmoChanged.Broadcast(CurrentAmmo, OldAmmo);
			bCanShoot = false;

			MuzzleFlash->Activate(true);
			GunFireSound->Play();
			if (GunFireAnim)
			{
				GetMesh()->GetAnimInstance()->Montage_Play(GunFireAnim);
			}

			GetWorld()->GetTimerManager().SetTimer(TimerHandle_ShootingTimer, this, &APyramidMathCharacter::ShotReady, ShootingRate);
			TraceShot();
		}
		else
		{
			GunDrySound->Play();
		}
	}
}

void APyramidMathCharacter::TraceShot()
{
	FVector TraceStart = GetActorLocation();
	FVector ShootDirection = LookPoint - TraceStart;
	ShootDirection.Normalize();
	// Lengthen the TraceEnd a little bit to make sure it hits the impact point just in case.
	FVector TraceEnd = LookPoint + ShootDirection * 100.0F;
	FHitResult Hit;
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, 0.1F);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.bReturnPhysicalMaterial = true;
	if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECollisionChannel::ECC_GameTraceChannel1, Params))
	{
		if (Hit.bBlockingHit)
		{
			// Spawn something on the shot location.
			DrawDebugSphere(GetWorld(), Hit.Location, 15.0F, 16, FColor::Red, false, .1F);
			/*UPrimitiveComponent* HitComp = Hit.GetComponent();
			UShootablePhysicalMaterial* PhysMat = Cast<UShootablePhysicalMaterial>(HitComp->GetBodyInstance()->GetSimplePhysicalMaterial());*/
			UShootablePhysicalMaterial* PhysMat = Cast<UShootablePhysicalMaterial>(Hit.PhysMaterial);
			if (PhysMat)
			{
				PhysMat->ShowImpact(Hit);
			}
			IShootable* Shootable = Cast<IShootable>(Hit.GetActor());
			if (Shootable)
			{
				Shootable->GetShot(Hit);
			}
		}
	}
}

void APyramidMathCharacter::CharacterBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	IPlayerInteractable* Interactable = Cast<IPlayerInteractable>(OtherActor);
	UE_LOG(LogTemp, Warning, TEXT("What the fuck?"));
	if (Interactable && Interactable->CanInteract(this) && Interactable->IsInteractableComponent(OtherComp))
	{
		UE_LOG(LogTemp, Warning, TEXT("Ready to interact"));
		InteractableActor = OtherActor;
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			AExplorerHUD* HUD = Cast<AExplorerHUD>(PC->GetHUD());
			if (HUD)
			{
				HUD->ShowAction(Interactable->GetInteractionName());
			}
		}
	}
	ATorchActor* TorchActor = Cast<ATorchActor>(OtherActor);
	if (TorchActor)
	{
		ActiveTorchActors.Add(TorchActor);
	}
}

void APyramidMathCharacter::CharacterEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == InteractableActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not interacting"));
		InteractableActor = nullptr;
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			AExplorerHUD* HUD = Cast<AExplorerHUD>(PC->GetHUD());
			if (HUD)
			{
				HUD->HideAction();
			}
		}
	}

	ATorchActor* TorchActor = Cast<ATorchActor>(OtherActor);
	if (ActiveTorchActors.Contains(TorchActor))
	{
		ActiveTorchActors.Remove(TorchActor);
	}
}
