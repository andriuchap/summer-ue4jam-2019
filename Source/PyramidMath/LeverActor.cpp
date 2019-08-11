// Fill out your copyright notice in the Description page of Project Settings.


#include "LeverActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "DoorActor.h"

// Sets default values
ALeverActor::ALeverActor(const FObjectInitializer& ObjInitializer)
{
	LeverSlotMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeverSlotMesh"));
	RootComponent = LeverSlotMesh;
	LeverSlotMesh->SetCollisionProfileName(FName("NoCollision"));

	LeverMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeverMesh"));
	LeverMesh->SetupAttachment(LeverSlotMesh);
	LeverMesh->SetCollisionProfileName(FName("NoCollision"));

	InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
	InteractionBox->SetupAttachment(LeverSlotMesh);
	InteractionBox->SetCollisionProfileName(FName("OverlapAllDynamic"));
	InteractionBox->SetGenerateOverlapEvents(true);

	bIsLeverPulled = false;
}

bool ALeverActor::IsInteractableComponent(UPrimitiveComponent * InOverlappedComponent)
{
	return true;
}

bool ALeverActor::CanInteract(APyramidMathCharacter * InCharacter)
{
	return !bIsLeverPulled;
}

bool ALeverActor::Interact(APyramidMathCharacter * InCharacter)
{
	OnLeverPulled();
	bIsLeverPulled = true;
	InteractionBox->SetCollisionProfileName(FName("NoCollision"));
	if (AssignedDoor)
	{
		AssignedDoor->OpenDoor();
	}
	return true;
}

FText ALeverActor::GetInteractionName()
{
	return NSLOCTEXT("PyramidMath", "INTERACTION_PULL_LEVER", "Pull Lever");
}

// Called when the game starts or when spawned
void ALeverActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALeverActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

