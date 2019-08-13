// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerInteractable.h"
#include "ProblemTablet.generated.h"

UCLASS()
class PYRAMIDMATH_API AProblemTablet : public AActor, public IPlayerInteractable
{
	GENERATED_BODY()
	
public:	
	AProblemTablet(const FObjectInitializer &ObjInitializer);

protected:
	UPROPERTY(EditAnywhere, Category = "Math Problem")
	class UMathProblem* Problem;

	UPROPERTY(VisibleDefaultsOnly, Category = "Math Problem")
		class UStaticMeshComponent* TabletMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "Math Problem")
		class UBoxComponent* InteractionBox;

	UPROPERTY(EditAnywhere, Category = "Math Problem")
		class ADoorActor* ProblemDoor;

	UPROPERTY(EditAnywhere, Category = "Math Problem")
		TSubclassOf<class UProblemUserWidget> ProblemWidgetClass;

private:
	UPROPERTY()
	UProblemUserWidget* ProblemWidget;

	bool bIsProblemSolved;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Math Problem")
		void OnProblemSolved();

	UFUNCTION(BlueprintImplementableEvent, Category = "Math Problem")
		void OnProblemWrong();

public:	
	virtual void Tick(float DeltaTime) override;

	virtual bool IsInteractableComponent(UPrimitiveComponent* InOverlappedComponent) override;
	virtual bool CanInteract(class APyramidMathCharacter* InCharacter) override;
	virtual bool Interact(APyramidMathCharacter* InCharacter) override;
	virtual FText GetInteractionName() override;

	void ProblemSolved();

	void ProblemWrong();
};
