// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PyramidMathCharacter.generated.h"

UCLASS(config=Game)
class APyramidMathCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	APyramidMathCharacter(const FObjectInitializer& ObjInitializer);
#pragma region Properties

public:

protected:
	UPROPERTY(EditAnywhere, Category = "Shooting")
	int32 AmmoCapacity;

	UPROPERTY(EditAnywhere, Category = "Shooting")
	float ShootingRate;

	UPROPERTY(EditAnywhere, Category = "Shooting")
	bool bInfiniteBullets;
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

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	void ShotReady();
	void Shoot();
	void TraceShot();

	UFUNCTION(BlueprintImplementableEvent, Category = "Shooting")
	void OnShotSuccess();

	UFUNCTION(BlueprintImplementableEvent, Category = "Shooting")
	void OnShotOutOfAmmo();

private:

#pragma endregion
};

