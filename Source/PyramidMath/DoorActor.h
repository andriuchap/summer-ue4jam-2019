// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorActor.generated.h"

UCLASS()
class PYRAMIDMATH_API ADoorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorActor(const FObjectInitializer &ObjInitializer);

protected:
	UPROPERTY(VisibleAnywhere, Category = "Door")
		class USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, Category = "Door")
		class UStaticMeshComponent* DoorMesh;

	UPROPERTY(VisibleAnywhere, Category = "Door")
		class UArrowComponent* OpenDoorLocation;

	UPROPERTY(EditAnywhere, Category = "Door")
		float DoorOpeningSpeed;

private:
	bool bIsDoorOpening;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Door")
		void OnDoorOpening();

	UFUNCTION(BlueprintImplementableEvent, Category = "Door")
		void OnDoorFinishOpening();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OpenDoor();

};
