// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerInteractable.h"
#include "TorchActor.generated.h"

UCLASS()
class PYRAMIDMATH_API ATorchActor : public AActor, public IPlayerInteractable
{
	GENERATED_BODY()
	
public:	
	ATorchActor(const FObjectInitializer& ObjInitializer);

#pragma region Properties
public:

protected:
	UPROPERTY(VisibleAnywhere, Category = "Torch")
		class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Torch")
		class UParticleSystemComponent* FlameParticleComponent;

	UPROPERTY(VisibleAnywhere, Category = "Torch")
		class UPointLightComponent* FlameLightComponent;

	UPROPERTY(VisibleAnywhere, Category = "Torch")
		class UAudioComponent* FlameLoopComponent;

	UPROPERTY(VisibleAnywhere, Category = "Torch")
		class USphereComponent* LightRadiusComponent;

	UPROPERTY(VisibleAnywhere, Category = "Torch")
		class UBoxComponent* InteractionComponent;

	UPROPERTY(EditAnywhere, Category = "Torch")
		float IgnitionFuelCost;

	UPROPERTY(EditAnywhere, Category = "Torch")
		bool bStartIgnited;

private:

	bool bIsTorchIgnited;
	float TorchIntensity;
	float FlickerIntensity;

	FTimerHandle TimerHandle_FlickerTimer;

#pragma endregion

#pragma region Functions
public:
	virtual void Tick(float DeltaTime) override;

	virtual bool IsInteractableComponent(class UPrimitiveComponent* InOverlappedComponent) override;
	virtual bool CanInteract(class APyramidMathCharacter* InCharacter) override;
	virtual bool Interact(APyramidMathCharacter* InCharacter) override;
	virtual FText GetInteractionName() override;

	bool IsIgnited();
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void FlickerLight();

private:
	void IgniteTorch();

#pragma endregion

};
