// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "ScarabDeathCounter.generated.h"

/**
 * 
 */
UCLASS()
class PYRAMIDMATH_API AScarabDeathCounter : public AInfo
{
	GENERATED_BODY()
public:
	AScarabDeathCounter(const FObjectInitializer &ObjInitializer);

protected:

	UPROPERTY(EditAnywhere, Category = "Death Counter")
		TArray<class AScarab*> Scarabs;

	UPROPERTY(EditAnywhere, Category = "Death Counter")
		class ADoorActor* Door;

private:

	int32 ScarabsRemain;

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnScarabDied();
};
