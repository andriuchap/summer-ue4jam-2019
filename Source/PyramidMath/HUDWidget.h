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
	UPROPERTY(meta = (BindWidget))
		class UBorder* ActionBorder;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ActionName;

private:

#pragma endregion

#pragma region Functions
public:

	void EnableAction(const FText& InText);
	void DisableAction();

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Pyramid")
	void AmmoChanged(int32 InCurrentAmmo, int32 InOldAmmo);

	UFUNCTION(BlueprintImplementableEvent, Category = "Pyramid")
	void HealthChanged(int32 InCurrentHealth, int32 OldHealth);

	UFUNCTION(BlueprintImplementableEvent, Category = "Pyramid")
	void GoldChanged(int32 InCurrentGold, int32 OldGold);

private:

#pragma endregion


};
