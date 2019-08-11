// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerInteractable.h"
#include "LeverActor.generated.h"

UCLASS()
class PYRAMIDMATH_API ALeverActor : public AActor, public IPlayerInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALeverActor(const FObjectInitializer& ObjInitializer);

protected:
	UPROPERTY(VisibleAnywhere, Category = "Lever")
	class UStaticMeshComponent* LeverSlotMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lever")
		UStaticMeshComponent* LeverMesh;

	UPROPERTY(VisibleAnywhere, Category = "Lever")
		class UBoxComponent* InteractionBox;

	UPROPERTY(EditAnywhere, Category = "Lever")
		class ADoorActor* AssignedDoor;

private:
	bool bIsLeverPulled;

public:
	virtual bool IsInteractableComponent(UPrimitiveComponent* InOverlappedComponent) override;
	virtual bool CanInteract(class APyramidMathCharacter* InCharacter) override;
	virtual bool Interact(APyramidMathCharacter* InCharacter) override;
	virtual FText GetInteractionName() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Lever")
		void OnLeverPulled();
};
