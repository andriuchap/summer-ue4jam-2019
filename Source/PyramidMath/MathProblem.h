// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MathProblem.generated.h"

/**
 * 
 */
UCLASS()
class PYRAMIDMATH_API UMathProblem : public UDataAsset
{
	GENERATED_BODY()
public:
	UMathProblem(const FObjectInitializer &ObjInitializer);

public:
	UPROPERTY(EditAnywhere, Category = "Math")
		TSoftObjectPtr<class UTexture2D> ProblemTexture;

	UPROPERTY(EditAnywhere, Category = "Math")
		TArray<TSoftObjectPtr<UTexture2D>> OptionTextures;

	UPROPERTY(EditAnywhere, Category = "Math")
		int32 CorrectSelection;
};
