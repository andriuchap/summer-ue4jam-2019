// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PyramidMathCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FHealthChangedDelegate, int32, int32);
DECLARE_MULTICAST_DELEGATE_TwoParams(FGoldChangedDelegate, int32, int32);
DECLARE_MULTICAST_DELEGATE_TwoParams(FAmmoChangedDelegate, int32, int32);

UCLASS(config=Game)
class APyramidMathCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	APyramidMathCharacter(const FObjectInitializer& ObjInitializer);
#pragma region Properties

public:

	FHealthChangedDelegate OnHealthChanged;
	FGoldChangedDelegate OnGoldChanged;
	FAmmoChangedDelegate OnAmmoChanged;

protected:
	UPROPERTY(EditAnywhere, Category = "Shooting")
	int32 AmmoCapacity;

	UPROPERTY(EditAnywhere, Category = "Shooting")
	float ShootingRate;

	UPROPERTY(EditAnywhere, Category = "Shooting")
	bool bInfiniteBullets;

	UPROPERTY(EditAnywhere, Category = "Torch")
	float TorchFuelCapacity;

	UPROPERTY(EditAnywhere, Category = "Torch")
	float TorchFuelDrainRate;

	UPROPERTY(EditAnywhere, Category = "Sanity")
	float MaxSanity;

	UPROPERTY(EditAnywhere, Category = "Sanity")
	float SanityRecoverRate;

	UPROPERTY(EditAnywhere, Category = "Sanity")
	float SanityDrainRate;

	UPROPERTY(EditAnywhere, Category = "Sanity")
	int32 MaxHealth;

	UPROPERTY(VisibleAnywhere, Category = "Items")
		class UStaticMeshComponent* Gun;

	UPROPERTY(EditAnywhere, Category = "Items")
		FName GunAttachBone;

	UPROPERTY(VisibleAnywhere, Category = "Items")
		class UParticleSystemComponent* MuzzleFlash;

	UPROPERTY(VisibleAnywhere, Category = "Items")
		class UAudioComponent* GunFireSound;

	UPROPERTY(VisibleAnywhere, Category = "Items")
		class UAudioComponent* GunDrySound;

	UPROPERTY(EditAnywhere, Category = "Items")
		class UAnimMontage* GunFireAnim;

	UPROPERTY(VisibleAnywhere, Category = "Items")
		UStaticMeshComponent* Torch;

	UPROPERTY(VisibleAnywhere, Category = "Items")
		UParticleSystemComponent* TorchFlame;

	UPROPERTY(VisibleAnywhere, Category = "Items")
		class UPointLightComponent* TorchLight;

	UPROPERTY(EditAnywhere, Category = "Items")
		FName TorchAttachBone;

	UPROPERTY(EditAnywhere, Category = "Items")
		float TorchIntensity;

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	FVector LookPoint;

	int32 CurrentAmmo;	

	FTimerHandle TimerHandle_ShootingTimer;

	bool bWantsToShoot;
	bool bCanShoot;

	bool bIsPebbleLoaded;

	float CurrentTorchFuel;
	bool bIsTorchIgnited;

	AActor* InteractableActor;

	float CurrentSanity;
	TArray<class ATorchActor*> ActiveTorchActors;

	int32 CurrentGold;

	int32 CurrentHealth;

#pragma endregion

#pragma region Functions
public:
	virtual void Tick(float DeltaSeconds) override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	void StartShooting();
	void StopShooting();

	void Interact();

	void LoadPebble();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Shooting")
	bool HasPebbleLoaded() const;
	void AddAmmo(int32 InAmount);
	int32 GetAmmo();
	int32 GetAmmoCapacity();

	void ToggleTorch();
	void AddTorchFuel(float InAmount);
	void RemoveTorchFuel(float InAmount);
	float GetTorchFuelAmount();
	float GetTorchFuelCapacity();

	float GetSanity();
	bool HasActiveTorchesAround();

	int32 GetGold();
	void AddGold(int32 InAmount);
	void RemoveGold(int32 InAmount);

	int32 GetHealth();
	int32 GetMaxHealth();
	void AddHealth(int32 InAmount);
	void DealDamage(int32 InAmount, FVector DamageDirection, float KnockbackMultiplier = 1.0F);
	UFUNCTION(BlueprintImplementableEvent, Category = "Pyramid Character")
	void OnDamaged();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Pyramid Character")
	void Die(bool bIsVoidDeath);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pyramid Character")
	bool IsDead();

	bool NeedsRestocking();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	virtual void BeginPlay() override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	void ShotReady();
	void Shoot();
	void TraceShot();

	UFUNCTION()
	void CharacterBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void CharacterEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:

#pragma endregion
};

