// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInteractable.generated.h"

UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UPlayerInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PYRAMIDMATH_API IPlayerInteractable
{
	GENERATED_BODY()
public:
	/** Used to check if the overlapped component is the one that's intended to be interacted with. */
	virtual bool IsInteractableComponent(UPrimitiveComponent* InOverlappedComponent) = 0;
	virtual bool CanInteract(class APyramidMathCharacter* InCharacter) = 0;
	virtual bool Interact(APyramidMathCharacter* InCharacter) = 0;
	virtual FText GetInteractionName() = 0;
};
