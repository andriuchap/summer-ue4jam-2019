// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorActor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
ADoorActor::ADoorActor(const FObjectInitializer &ObjInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetCollisionProfileName(FName("BlockAllDynamic"));
	DoorMesh->SetupAttachment(SceneRoot);

	OpenDoorLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("OpenDoorLocation"));
	OpenDoorLocation->SetupAttachment(SceneRoot);

	DoorOpeningSpeed = 100.0F;
}

// Called when the game starts or when spawned
void ADoorActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsDoorOpening)
	{
		FVector MoveDirection = OpenDoorLocation->GetComponentLocation() - DoorMesh->GetComponentLocation();
		float MoveDelta = DoorOpeningSpeed * DeltaTime;
		if (MoveDelta >= MoveDirection.Size())
		{
			DoorMesh->SetWorldLocation(OpenDoorLocation->GetComponentLocation());
			bIsDoorOpening = false;
			OnDoorFinishOpening();
		}
		else
		{
			MoveDirection.Normalize();
			DoorMesh->AddWorldOffset(MoveDirection * MoveDelta);
		}
	}
}

void ADoorActor::OpenDoor()
{
	bIsDoorOpening = true;
	OnDoorOpening();
}

