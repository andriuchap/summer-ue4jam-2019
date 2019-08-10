// Fill out your copyright notice in the Description page of Project Settings.


#include "RestockArea.h"
#include "Engine/World.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PlayerController.h"
#include "PyramidMathCharacter.h"

ARestockArea::ARestockArea(const FObjectInitializer& ObjInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	RestockAreaOverlap = CreateDefaultSubobject<UBoxComponent>(TEXT("RestockAreaOverlap"));
	RootComponent = RestockAreaOverlap;
	RestockAreaOverlap->SetCollisionProfileName(FName("OverlapAllDynamic"));
	RestockAreaOverlap->SetGenerateOverlapEvents(true);

	HealthUnitPrice = 100;
	AmmoUnitPrice = 10;
	FuelUnitPrice = 1;
}

bool ARestockArea::IsInteractableComponent(UPrimitiveComponent * InOverlappedComponent)
{
	return true;
}

bool ARestockArea::CanInteract(APyramidMathCharacter* InCharacter)
{
	if (InCharacter)
	{
		return InCharacter->NeedsRestocking() && CanAffordRestock(InCharacter);
	}
	return false;
}

bool ARestockArea::Interact(APyramidMathCharacter* InCharacter)
{
	int32 CharacterRemainingGold = InCharacter->GetGold();
	int32 TotalPrice = 0;
	int32 MissingHealth = InCharacter->GetMaxHealth() - InCharacter->GetHealth();
	if (MissingHealth > 0)
	{
		int32 PriceToPay = MissingHealth * HealthUnitPrice;
		int32 ReceivedHealth = MissingHealth;
		if (CharacterRemainingGold < PriceToPay)
		{
			// How much health can the player afford?
			ReceivedHealth = FMath::FloorToInt((float)CharacterRemainingGold / (float)HealthUnitPrice);
			PriceToPay = ReceivedHealth * HealthUnitPrice;
		}
		CharacterRemainingGold -= PriceToPay;
		TotalPrice += PriceToPay;
		if (ReceivedHealth > 0)
		{
			InCharacter->AddHealth(ReceivedHealth);
		}
	}

	int32 MissingAmmo = InCharacter->GetAmmoCapacity() - InCharacter->GetAmmo();
	if (MissingAmmo > 0)
	{
		int32 PriceToPay = MissingAmmo * AmmoUnitPrice;
		int32 ReceivedAmmo = MissingAmmo;
		if (CharacterRemainingGold < PriceToPay)
		{
			// How much ammo can he afford?
			ReceivedAmmo = FMath::FloorToInt((float)CharacterRemainingGold / (float)AmmoUnitPrice);
			PriceToPay = ReceivedAmmo * AmmoUnitPrice;
		}
		CharacterRemainingGold -= PriceToPay;
		TotalPrice += PriceToPay;
		if (ReceivedAmmo > 0)
		{
			InCharacter->AddAmmo(ReceivedAmmo);
		}
	}

	float MissingFuel = InCharacter->GetTorchFuelCapacity() - InCharacter->GetTorchFuelAmount();
	if (MissingFuel > 0.0F)
	{
		int32 PriceToPay = FMath::CeilToInt(MissingFuel) * FuelUnitPrice;
		float ReceivedFuel = MissingFuel;
		if (CharacterRemainingGold < PriceToPay)
		{
			ReceivedFuel = FMath::FloorToInt((float)CharacterRemainingGold / (float)FuelUnitPrice);
			PriceToPay = ReceivedFuel * FuelUnitPrice;
		}
		CharacterRemainingGold -= PriceToPay;
		TotalPrice += PriceToPay;
		if (ReceivedFuel > 0.0F)
		{
			InCharacter->AddTorchFuel(ReceivedFuel);
		}
	}

	if (TotalPrice > 0)
	{
		InCharacter->RemoveGold(TotalPrice);
	}

	return false;
}

FText ARestockArea::GetInteractionName()
{
	return NSLOCTEXT("PyramidMath", "INTERACTION_RESTOCK", "Restock");
}

void ARestockArea::BeginPlay()
{
	Super::BeginPlay();

}

bool ARestockArea::CanAffordRestock(APyramidMathCharacter * InCharacter)
{
	int32 CharacterGold = InCharacter->GetGold();
	int32 MissingAmmo = InCharacter->GetAmmoCapacity() - InCharacter->GetAmmo();
	// Needs atleast one bullet and can afford it
	if (MissingAmmo > 0 && CharacterGold >= AmmoUnitPrice)
	{
		return true;
	}
	int32 MissingHealth = InCharacter->GetMaxHealth() - InCharacter->GetHealth();
	if (MissingHealth > 0 && CharacterGold >= HealthUnitPrice)
	{
		return true;
	}
	float MissingFuel = InCharacter->GetTorchFuelCapacity() - InCharacter->GetTorchFuelAmount();
	if (MissingFuel > 0.0F && CharacterGold >= FuelUnitPrice)
	{
		return true;
	}

	return false;
}
