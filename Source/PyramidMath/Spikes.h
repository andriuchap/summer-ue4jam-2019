// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Spikes.generated.h"

/**
 * 
 */
UCLASS()
class PYRAMIDMATH_API ASpikes : public AStaticMeshActor
{
	GENERATED_BODY()
public:
	ASpikes(const FObjectInitializer& ObjInitializer);

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void SpikesBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

};
