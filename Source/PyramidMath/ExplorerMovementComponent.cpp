// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplorerMovementComponent.h"
#include "GameFramework/PhysicsVolume.h"

UExplorerMovementComponent::UExplorerMovementComponent(const FObjectInitializer &ObjInitializer)
{
	FallingGravityScale = 2.0F;
	FallingThreshold = 100.0F;
}

void UExplorerMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
}

FVector UExplorerMovementComponent::NewFallVelocity(const FVector& InitialVelocity, const FVector& Gravity, float DeltaTime) const
{
	FVector Result = InitialVelocity;

	if (DeltaTime > 0.f)
	{
		if (InitialVelocity.Z < FallingThreshold)
		{
			// Apply extra gravity when falling so the character doesn't feel as floaty.
			float Multiplier = 1 - (FMath::Clamp(InitialVelocity.Z, 0.0F, FallingThreshold) / FallingThreshold);
			Result += Gravity * DeltaTime * FallingGravityScale;
		}
		else
		{
			// Apply gravity.
			Result += Gravity * DeltaTime;
		}

		// Don't exceed terminal velocity.
		const float TerminalLimit = FMath::Abs(GetPhysicsVolume()->TerminalVelocity);
		if (Result.SizeSquared() > FMath::Square(TerminalLimit))
		{
			const FVector GravityDir = Gravity.GetSafeNormal();
			if ((Result | GravityDir) > TerminalLimit)
			{
				Result = FVector::PointPlaneProject(Result, FVector::ZeroVector, GravityDir) + GravityDir * TerminalLimit;
			}
		}
	}

	return Result;
}