// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "ShootablePhysicalMaterial.generated.h"

/**
 * 
 */
UCLASS()
class PYRAMIDMATH_API UShootablePhysicalMaterial : public UPhysicalMaterial
{
	GENERATED_BODY()
public:
	UShootablePhysicalMaterial(const FObjectInitializer& ObjInitializer);

#pragma region Properties
public:

protected:
	UPROPERTY(EditAnywhere, Category = "Shootable")
		class UParticleSystem* ImpactParticle;

	UPROPERTY(EditAnywhere, Category = "Shootable")
		class USoundBase* ImpactSound;

	UPROPERTY(EditAnywhere, Category = "Shootable")
		TSubclassOf<class AImpactDecalActor> ImpactDecalClass;

private:

#pragma endregion

#pragma region Functions
public:
	void ShowImpact(const FHitResult& Hit);
protected:

private:

#pragma endregion

};
