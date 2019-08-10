// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UtilTypes.generated.h"

USTRUCT()
struct FMinMaxRange
{
	GENERATED_BODY()
public:
	UPROPERTY()
		float Min;

	UPROPERTY()
		float Max;

	FMinMaxRange() : Min(0.0F), Max(0.0F) { }
};
