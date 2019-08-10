// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerInteractable.h"
#include "SupplyContainer.generated.h"

UCLASS()
class PYRAMIDMATH_API ASupplyContainer : public AActor, public IPlayerInteractable
{
	GENERATED_BODY()	
public:	
	ASupplyContainer(const FObjectInitializer& ObjInitializer);

#pragma region Properties
public:
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "Supplies")
		class UStaticMeshComponent* SupplyMesh;

	UPROPERTY(VisibleAnywhere, Category = "Supplies")
		class UBoxComponent* InteractionBox;

	UPROPERTY(EditAnywhere, Category = "Supplies")
		float ContainedTorchFuel;

	UPROPERTY(EditAnywhere, Category = "Supplies")
		int32 ContainedAmmo;

	UPROPERTY(EditAnywhere, Category = "Supplies")
		int32 ContainedHealth;

private:
	bool bIsLooted;

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


#pragma endregion
};
