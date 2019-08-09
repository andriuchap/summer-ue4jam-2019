// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ExplorerHUD.generated.h"

/**
 * 
 */
UCLASS()
class PYRAMIDMATH_API AExplorerHUD : public AHUD
{
	GENERATED_BODY()
public:
	AExplorerHUD(const FObjectInitializer &ObjInitializer);

#pragma region Properties
public:

protected:
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<class UHUDWidget> HUDWidgetClass;
private:
	UPROPERTY()
	UHUDWidget* HUDWidget;

#pragma endregion

#pragma region Functions
public:
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

private:

#pragma endregion

};
