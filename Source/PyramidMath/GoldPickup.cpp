// Fill out your copyright notice in the Description page of Project Settings.


#include "GoldPickup.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PyramidMathCharacter.h"

AGoldPickup::AGoldPickup(const FObjectInitializer& ObjInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;
	StaticMeshComponent->SetCollisionProfileName(FName("OverlapAllDynamic"));
	StaticMeshComponent->SetGenerateOverlapEvents(true);
	StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AGoldPickup::MeshBeginOverlap);

	IdleParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IdleParticleSystem"));
	IdleParticleSystem->SetupAttachment(StaticMeshComponent);
}

void AGoldPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGoldPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGoldPickup::MeshBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	APyramidMathCharacter* Character = Cast<APyramidMathCharacter>(OtherActor);
	if (Character)
	{
		Character->AddGold(GoldValue);
		OnPickedUp();
		Destroy();
	}
}
