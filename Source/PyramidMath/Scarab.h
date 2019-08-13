// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Shootable.h"
#include "Navigation/PathFollowingComponent.h"
#include "Scarab.generated.h"

DECLARE_MULTICAST_DELEGATE(FScarabDiedDelegate);

UCLASS()
class PYRAMIDMATH_API AScarab : public ACharacter, public IShootable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AScarab(const FObjectInitializer& ObjInitializer);

protected:

	UPROPERTY(EditAnywhere, Category = "Scarab")
		float ScanningRate;

	UPROPERTY(EditAnywhere, Category = "Scarab")
		float ScanningRange;

	UPROPERTY(EditAnywhere, Category = "Scarab")
		int32 MaxHealth;

	UPROPERTY(EditAnywhere, Category = "Scarab")
		class UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, Category = "Scarab")
		class UAnimMontage* DeathMontage;

private:
	bool bIsChasingPlayer;

	FTimerHandle TimerHandle_ScanningTimer;
	FTimerHandle TimerHandle_ChaseTimer;

	int32 Health;

	bool bIsDead;

	AActor* Target;

	EPathFollowingStatus::Type LastPathStatus;

protected:
	virtual void BeginPlay() override;

	void ScanForPlayer();

	void GoToPlayer();

	void EvaluateChaseState();

	void ClearScarab();

	UFUNCTION(BlueprintImplementableEvent, Category = "Scarab")
	void OnSuccessfulHit();
public:	
	virtual void Tick(float DeltaTime) override;

	virtual void GetShot(const FHitResult& ShotHit) override;

	UFUNCTION(BlueprintCallable, Category = "Scarab")
	void HitPlayer();

	FScarabDiedDelegate OnScarabDied;
};
