// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class PYRAMIDMATH_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UHUDWidget(const FObjectInitializer& ObjInitializer);

#pragma region Properties
public:

protected:

private:

#pragma endregion

#pragma region Functions
public:

protected:
	virtual void NativeConstruct() override;

private:

#pragma endregion


};
