// Fill out your copyright notice in the Description page of Project Settings.


#include "MathProblem.h"
#include "Engine/Texture2D.h"

UMathProblem::UMathProblem(const FObjectInitializer &ObjInitializer)
{
	ProblemTexture = nullptr;
	OptionTextures = TArray <TSoftObjectPtr<UTexture2D>>();
	OptionTextures.SetNum(3);
	CorrectSelection = 0;
}

