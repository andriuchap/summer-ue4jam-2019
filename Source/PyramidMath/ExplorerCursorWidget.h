// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExplorerCursorWidget.generated.h"

/**
 * 
 */
UCLASS()
class PYRAMIDMATH_API UExplorerCursorWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UExplorerCursorWidget(const FObjectInitializer& ObjInitializer);

#pragma region Properties
public:

protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* CursorImage;
private:

#pragma endregion

#pragma region Functions
public:

protected:

private:

#pragma endregion

};
