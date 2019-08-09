// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ExplorerMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class PYRAMIDMATH_API UExplorerMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:
	UExplorerMovementComponent(const FObjectInitializer &ObjInitializer);

#pragma region Properties
public:

protected:
	UPROPERTY(EditAnywhere, Category = "Movement")
	float FallingGravityScale;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float FallingThreshold;

private:

#pragma endregion

#pragma region Functions
public:
	virtual FVector NewFallVelocity(const FVector& InitialVelocity, const FVector& Gravity, float DeltaTime) const;
	
protected:
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode);

private:

#pragma endregion


};
