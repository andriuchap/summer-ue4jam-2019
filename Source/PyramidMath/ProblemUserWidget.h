// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProblemUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PYRAMIDMATH_API UProblemUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UProblemUserWidget(const FObjectInitializer &ObjInitializer);

protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* ProblemImage;

	UPROPERTY(BlueprintReadOnly, Category = "Math Problem")
	TArray<UImage*> SolutionImages;

private:
	int32 CorrectIndex;

	class AProblemTablet* ProblemTablet;

protected:
	UFUNCTION(BlueprintCallable, Category = "Math Problem")
		void SelectSolution(int InIndex);

public:

	void SetProblem(class UMathProblem* InProblem);
	void SetTablet(AProblemTablet* InTablet);
};
