// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DecalActor.h"
#include "ImpactDecalActor.generated.h"

/**
 * 
 */
UCLASS()
class PYRAMIDMATH_API AImpactDecalActor : public ADecalActor
{
	GENERATED_BODY()
public:
	AImpactDecalActor(const FObjectInitializer& ObjInitializer);
};
