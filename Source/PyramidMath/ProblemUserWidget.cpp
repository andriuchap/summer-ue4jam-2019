// Fill out your copyright notice in the Description page of Project Settings.


#include "ProblemUserWidget.h"
#include "MathProblem.h"
#include "UMG/Public/Components/Image.h"
#include "ProblemTablet.h"

UProblemUserWidget::UProblemUserWidget(const FObjectInitializer &ObjInitializer) : Super(ObjInitializer)
{

}

void UProblemUserWidget::SelectSolution(int InIndex)
{
	if (ProblemTablet)
	{
		if (InIndex == CorrectIndex)
		{
			ProblemTablet->ProblemSolved();
		}
		else
		{
			ProblemTablet->ProblemWrong();
		}
	}
}

void UProblemUserWidget::SetProblem(UMathProblem * InProblem)
{
	if (InProblem)
	{
		//if (InProblem->ProblemTexture)
		//{
			UTexture2D* ProblemTexture = InProblem->ProblemTexture.LoadSynchronous();
			ProblemImage->SetBrushFromTexture(ProblemTexture);
		//}
		TArray<int32> UnshuffeledIndices = TArray<int32>();
		for (int i = 0; i < 3; i++)
		{
			UnshuffeledIndices.Add(i);
		}
		TArray<int32> ShuffeledIndices = TArray<int32>();
		while (UnshuffeledIndices.Num() > 0)
		{
			int32 RandomIndex = FMath::RandRange(0, UnshuffeledIndices.Num() - 1);
			int32 ShuffeledIndex = UnshuffeledIndices[RandomIndex];
			ShuffeledIndices.Add(ShuffeledIndex);
			UnshuffeledIndices.Remove(ShuffeledIndex);
		}
		for (int i = 0; i < InProblem->OptionTextures.Num(); i++)
		{
			int32 Index = ShuffeledIndices[i];
			UTexture2D* SolutionTexture = InProblem->OptionTextures[Index].LoadSynchronous();
			SolutionImages[i]->SetBrushFromTexture(SolutionTexture);
			if (Index == InProblem->CorrectSelection)
			{
				CorrectIndex = i;
			}
		}
	}
}

void UProblemUserWidget::SetTablet(AProblemTablet * InTablet)
{
	ProblemTablet = InTablet;
}