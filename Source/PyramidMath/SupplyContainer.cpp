// Fill out your copyright notice in the Description page of Project Settings.

#include "SupplyContainer.h"
#include "PyramidMathCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

ASupplyContainer::ASupplyContainer(const FObjectInitializer& ObjInitializer)
{
	bIsLooted = false;

	SupplyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SupplyMesh"));
	RootComponent = SupplyMesh;
	SupplyMesh->SetCollisionProfileName(FName("NoCollision"));

	InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
	InteractionBox->SetupAttachment(SupplyMesh);
	InteractionBox->SetCollisionProfileName(FName("OverlapAllDynamic"));
	InteractionBox->SetGenerateOverlapEvents(true);
}

bool ASupplyContainer::IsInteractableComponent(UPrimitiveComponent * InOverlappedComponent)
{
	return true;
}

bool ASupplyContainer::CanInteract(APyramidMathCharacter * InCharacter)
{
	return !bIsLooted;
}

bool ASupplyContainer::Interact(APyramidMathCharacter * InCharacter)
{
	bIsLooted = true;
	if (InCharacter)
	{
		InCharacter->AddAmmo(ContainedAmmo);
		InCharacter->AddHealth(ContainedHealth);
		InCharacter->AddTorchFuel(ContainedTorchFuel);
	}
	return true;
}

FText ASupplyContainer::GetInteractionName()
{
	return NSLOCTEXT("PyramidMath", "INTERACTION_SEARCH_SUPPLIES", "Search");
}

void ASupplyContainer::BeginPlay()
{
	Super::BeginPlay();
	
}