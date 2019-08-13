// Fill out your copyright notice in the Description page of Project Settings.

#include "ProblemTablet.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "DoorActor.h"
#include "ProblemUserWidget.h"
#include "PyramidMathCharacter.h"

AProblemTablet::AProblemTablet(const FObjectInitializer &ObjInitializer)
{
	Problem = nullptr;

	TabletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TabletMesh"));
	RootComponent = TabletMesh;
	TabletMesh->SetCollisionProfileName(FName("BlockAllDynamic"));

	InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
	InteractionBox->SetupAttachment(TabletMesh);
	InteractionBox->SetCollisionProfileName(FName("OverlapAllDynamic"));
	InteractionBox->SetGenerateOverlapEvents(true);

	ProblemWidgetClass = UProblemUserWidget::StaticClass();
	ProblemWidget = nullptr;

	bIsProblemSolved = false;
}

void AProblemTablet::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProblemTablet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AProblemTablet::IsInteractableComponent(UPrimitiveComponent * InOverlappedComponent)
{
	return true;
}

bool AProblemTablet::CanInteract(APyramidMathCharacter * InCharacter)
{
	return !bIsProblemSolved;
}

bool AProblemTablet::Interact(APyramidMathCharacter * InCharacter)
{
	APlayerController* PC = Cast<APlayerController>(InCharacter->GetController());
	if (PC)
	{
		ProblemWidget = CreateWidget<UProblemUserWidget>(PC, ProblemWidgetClass);
		ProblemWidget->AddToViewport();
		PC->GetPawn()->DisableInput(PC);
		ProblemWidget->SetProblem(Problem);
		ProblemWidget->SetTablet(this);
	}
	return true;
}

FText AProblemTablet::GetInteractionName()
{
	return NSLOCTEXT("PyramidMath", "INTERACTION_READ_TABLET", "Read");
}

void AProblemTablet::ProblemSolved()
{
	if (ProblemDoor)
	{
		ProblemDoor->OpenDoor();
	}
	bIsProblemSolved = true;
	InteractionBox->SetCollisionProfileName(FName("NoCollision"));

	APlayerController* PC = ProblemWidget->GetOwningPlayer();
	if (PC)
	{
		PC->GetPawn()->EnableInput(PC);
		ProblemWidget->RemoveFromParent();
		ProblemWidget = nullptr;
	}
	OnProblemSolved();
}

void AProblemTablet::ProblemWrong()
{
	APlayerController* PC = ProblemWidget->GetOwningPlayer();
	if (PC)
	{
		PC->GetPawn()->EnableInput(PC);
		ProblemWidget->RemoveFromParent();
		ProblemWidget = nullptr;
	}

	OnProblemWrong();
}

