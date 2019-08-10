// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerInteractable.h"
#include "RestockArea.generated.h"

UCLASS()
class PYRAMIDMATH_API ARestockArea : public AActor, public IPlayerInteractable
{
	GENERATED_BODY()
	
public:	
	ARestockArea(const FObjectInitializer& ObjInitializer);

#pragma region Properties
public:

protected:
	UPROPERTY(VisibleAnywhere, Category = "Restock")
	class UBoxComponent* RestockAreaOverlap;

	UPROPERTY(EditAnywhere, Category = "Restock")
		int32 HealthUnitPrice;

	UPROPERTY(EditAnywhere, Category = "Restock")
		int32 AmmoUnitPrice;

	UPROPERTY(EditAnywhere, Category = "Restock")
		int32 FuelUnitPrice;

private:

#pragma endregion

#pragma region Functions
public:

	virtual bool IsInteractableComponent(UPrimitiveComponent* InOverlappedComponent) override;
	virtual bool CanInteract(class APyramidMathCharacter* InCharacter) override;
	virtual bool Interact(APyramidMathCharacter* InCharacter) override;
	virtual FText GetInteractionName() override;

protected:
	virtual void BeginPlay() override;

private:

	bool CanAffordRestock(APyramidMathCharacter* InCharacter);

#pragma endregion
};
