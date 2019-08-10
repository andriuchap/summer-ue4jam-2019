// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PyramidMathCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "ExplorerMovementComponent.h"
#include "TimerManager.h"
#include "Shootable.h"
#include "ShootablePhysicalMaterial.h"
#include "PlayerInteractable.h"
#include "TorchActor.h"
#include "DrawDebugHelpers.h"

APyramidMathCharacter::APyramidMathCharacter(const FObjectInitializer& ObjInitializer) : Super(ObjInitializer.SetDefaultSubobjectClass<UExplorerMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APyramidMathCharacter::CharacterBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &APyramidMathCharacter::CharacterEndOverlap);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

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

	CurrentAmmo = 0;
	bWantsToShoot = false;
	bCanShoot = true;
	bIsPebbleLoaded = false;

	CurrentTorchFuel = TorchFuelCapacity;
	bIsTorchIgnited = false;

	InteractableActor = nullptr;

	CurrentSanity = MaxSanity;
}

void APyramidMathCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
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
				PC->SetControlRotation(NewRotation);
			}
		}
	}
	if (bIsTorchIgnited)
	{
		CurrentTorchFuel -= TorchFuelDrainRate * DeltaSeconds;
		if (CurrentTorchFuel <= 0.0F)
		{
			CurrentTorchFuel = 0.0F;
			bIsTorchIgnited = false;
		}
	}
	if (bIsTorchIgnited || HasActiveTorchesAround())
	{
		CurrentSanity = FMath::Clamp<float>(CurrentSanity + SanityRecoverRate * DeltaSeconds, 0.0F, MaxSanity);
		UE_LOG(LogTemp, Warning, TEXT("Recovering sanity: %f"), CurrentSanity);
	}
	else
	{
		CurrentSanity -= SanityDrainRate * DeltaSeconds;
		if (CurrentSanity <= 0.0F)
		{
			CurrentSanity = 0.0F;
			// Announce player's death!
		}
		UE_LOG(LogTemp, Warning, TEXT("Going insane!: %f"), CurrentSanity);
	}
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
		if (Interactable->CanInteract())
		{
			Interactable->Interact();
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
	CurrentAmmo = FMath::Clamp<int32>(CurrentAmmo + InAmount, 0, AmmoCapacity);
}

int32 APyramidMathCharacter::GetAmmo()
{
	return CurrentAmmo;
}

void APyramidMathCharacter::ToggleTorch()
{
	bIsTorchIgnited = !bIsTorchIgnited;
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
			CurrentAmmo = FMath::Clamp<int32>(CurrentAmmo - 1, 0, AmmoCapacity);
			bCanShoot = false;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_ShootingTimer, this, &APyramidMathCharacter::ShotReady, ShootingRate);
			TraceShot();
			OnShotSuccess();
		}
		else
		{
			OnShotOutOfAmmo();
		}
	}
}

void APyramidMathCharacter::TraceShot()
{
	FVector TraceStart = GetActorLocation() + GetActorForwardVector() * 100.0F;
	// Shoot where the cursor is aiming.
	FVector ShootDirection = LookPoint - TraceStart;
	ShootDirection.Normalize();
	// Lengthen the TraceEnd a little bit to make sure it hits the impact point just in case.
	FVector TraceEnd = LookPoint + ShootDirection * 100.0F;
	// Shoot forward to where the character is facing.
	//FVector TraceEnd = GetActorLocation() + GetActorForwardVector() * 2000.0F;
	FHitResult Hit;
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, 0.1F);
	if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECollisionChannel::ECC_GameTraceChannel1))
	{
		if (Hit.bBlockingHit)
		{
			// Spawn something on the shot location.
			DrawDebugSphere(GetWorld(), Hit.Location, 15.0F, 16, FColor::Red, false, .1F);
			UPrimitiveComponent* HitComp = Hit.GetComponent();
			UShootablePhysicalMaterial* PhysMat = Cast<UShootablePhysicalMaterial>(HitComp->GetBodyInstance()->GetSimplePhysicalMaterial());
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
	if (Interactable && Interactable->CanInteract() && Interactable->IsInteractableComponent(OtherComp))
	{
		InteractableActor = OtherActor;
	}
	ATorchActor* TorchActor = Cast<ATorchActor>(OtherActor);
	if (TorchActor)
	{
		ActiveTorchActors.Add(TorchActor);
	}
}

void APyramidMathCharacter::CharacterEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (InteractableActor && OtherActor == InteractableActor)
	{
		InteractableActor = nullptr;
	}

	ATorchActor* TorchActor = Cast<ATorchActor>(OtherActor);
	if (ActiveTorchActors.Contains(TorchActor))
	{
		ActiveTorchActors.Remove(TorchActor);
	}
}
