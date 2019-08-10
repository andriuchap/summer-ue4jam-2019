// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GoldPickup.generated.h"

UCLASS()
class PYRAMIDMATH_API AGoldPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	AGoldPickup(const FObjectInitializer& ObjInitializer);

#pragma region Properties
public:

protected:
	UPROPERTY(VisibleAnywhere, Category = "Gold")
		class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Gold")
		class UParticleSystemComponent* IdleParticleSystem;

	UPROPERTY(EditAnywhere, Category = "Gold")
		int32 GoldValue;

private:

#pragma endregion


#pragma region Functions
public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void MeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintImplementableEvent, Category = "Gold")
		void OnPickedUp();

private:

#pragma endregion
};
