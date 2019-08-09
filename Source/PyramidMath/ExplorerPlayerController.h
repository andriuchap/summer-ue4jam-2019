// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ExplorerPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PYRAMIDMATH_API AExplorerPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AExplorerPlayerController(const FObjectInitializer &ObjInitializer);

#pragma region Properties
public:

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Explorer")
		TSubclassOf<class UExplorerCursorWidget> CursorWidgetClass;
private:
	UPROPERTY()
	UExplorerCursorWidget* CursorWidget;

#pragma endregion

#pragma region Functions
public:

protected:
	virtual void BeginPlay() override;

private:

#pragma endregion

};
