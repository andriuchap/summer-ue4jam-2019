// Fill out your copyright notice in the Description page of Project Settings.


#include "Scarab.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "AIController.h"
#include "PyramidMathCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"

AScarab::AScarab(const FObjectInitializer& ObjInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	bIsChasingPlayer = false;

	ScanningRate = 1.0F;

	ScanningRange = 100.0F;

	MaxHealth = 2;
	Health = MaxHealth;

	bIsDead = false;
}

void AScarab::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_ScanningTimer, this, &AScarab::ScanForPlayer, ScanningRate, true);
	Health = MaxHealth;
}

void AScarab::ScanForPlayer()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		APawn* Pawn = PC->GetPawn();
		if (Pawn)
		{
			FVector PawnLocation = Pawn->GetActorLocation();
			FVector ScarabLocation = GetActorLocation();
			FVector Direction = PawnLocation - ScarabLocation;
			Direction.Normalize();
			FVector TraceStart = ScarabLocation;
			FVector TraceEnd = ScarabLocation + Direction * ScanningRange;
			FHitResult Hit;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this);
			if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Pawn, Params))
			{
				if (Hit.bBlockingHit)
				{
					AActor* HitActor = Hit.GetActor();
					if (HitActor == Pawn)
					{
						bIsChasingPlayer = true;
						GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ScanningTimer);
						Target = Pawn;
						GoToPlayer();
						GetWorld()->GetTimerManager().SetTimer(TimerHandle_ChaseTimer, this, &AScarab::EvaluateChaseState, 1.0F, true);
					}
				}
			}
		}
	}
}

void AScarab::GoToPlayer()
{
	if (bIsChasingPlayer && !bIsDead)
	{
		AAIController* AI = Cast<AAIController>(GetController());
		if (AI && Target)
		{
			AI->MoveToActor(Target, 50.0F);
		}
	}
}

void AScarab::EvaluateChaseState()
{
	if (bIsChasingPlayer && !bIsDead)
	{
		AAIController* AI = Cast<AAIController>(GetController());
		if (AI)
		{
			EPathFollowingStatus::Type PathFollowingStatus = AI->GetPathFollowingComponent()->GetStatus();
			if (PathFollowingStatus == EPathFollowingStatus::Idle && LastPathStatus == EPathFollowingStatus::Idle)
			{
				GoToPlayer();
			}
		}
	}
}

void AScarab::ClearScarab()
{
	OnScarabDied.Clear();
	Destroy();
}

void AScarab::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsChasingPlayer && !bIsDead)
	{
		AAIController* AI = Cast<AAIController>(GetController());
		if (AI)
		{
			EPathFollowingStatus::Type PathFollowingStatus = AI->GetPathFollowingComponent()->GetStatus();
			if (PathFollowingStatus == EPathFollowingStatus::Idle && LastPathStatus == EPathFollowingStatus::Moving)
			{
				// HIT THE PLAYER
				FTimerHandle DummyTimer;
				GetWorld()->GetTimerManager().SetTimer(DummyTimer, this, &AScarab::HitPlayer, 0.5F);
				if (AttackMontage)
				{
					UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
					if (AnimInstance)
					{
						AnimInstance->Montage_Play(AttackMontage);
					}
				}
			}
			LastPathStatus = PathFollowingStatus;
		}
	}
}

void AScarab::GetShot(const FHitResult & ShotHit)
{
	Health--;
	FVector KnockbackDirection = ShotHit.TraceEnd - ShotHit.TraceStart + FVector::UpVector * 100.0F;
	KnockbackDirection.Normalize();
	FVector Knockback = KnockbackDirection * 250.0F;
	LaunchCharacter(Knockback, true, true);
	if (Health == 0)
	{
		bIsDead = true;
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ScanningTimer);
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ChaseTimer);
		AAIController* AI = Cast<AAIController>(GetController());
		if (AI)
		{
			AI->StopMovement();
		}
		if (DeathMontage)
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				AnimInstance->Montage_Play(DeathMontage);
			}
		}
		OnScarabDied.Broadcast();
		FTimerHandle TimerHandle_ClearScarab;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_ClearScarab, this, &AScarab::ClearScarab, 2.5F);
	}
}

void AScarab::HitPlayer()
{
	FVector TraceStart = GetActorLocation();
	FVector TraceEnd = GetActorLocation() + GetActorForwardVector() * 100.0F;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Pawn, Params))
	{
		if (Hit.bBlockingHit)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor == Target)
			{
				APyramidMathCharacter* Character = Cast<APyramidMathCharacter>(HitActor);
				Character->DealDamage(1, GetActorForwardVector());
				OnSuccessfulHit();
			}
		}
	}

	GoToPlayer();
}