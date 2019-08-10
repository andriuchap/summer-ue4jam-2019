// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidget.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/CanvasPanelSlot.h"
#include "PyramidMathCharacter.h"

UHUDWidget::UHUDWidget(const FObjectInitializer& ObjInitializer) : Super(ObjInitializer)
{

}

void UHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	APyramidMathCharacter* Character = Cast<APyramidMathCharacter>(GetOwningPlayer()->GetPawn());
	if (Character)
	{
		Character->OnAmmoChanged.AddUFunction(this, FName("AmmoChanged"));
		Character->OnHealthChanged.AddUFunction(this, FName("HealthChanged"));
		Character->OnGoldChanged.AddUFunction(this, FName("GoldChanged"));

		AmmoChanged(Character->GetAmmo(), Character->GetAmmo());
		HealthChanged(Character->GetHealth(), Character->GetHealth());
		GoldChanged(Character->GetGold(), Character->GetGold());
	}
}
