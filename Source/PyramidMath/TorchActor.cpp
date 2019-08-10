// Fill out your copyright notice in the Description page of Project Settings.


#include "TorchActor.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PlayerController.h"
#include "PyramidMathCharacter.h"
#include "TimerManager.h"

ATorchActor::ATorchActor(const FObjectInitializer& ObjInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;

	FlameParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FlameParticleComponent"));
	FlameParticleComponent->SetupAttachment(StaticMeshComponent);
	FlameParticleComponent->bAutoActivate = false;

	FlameLightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("FlameLightComponent"));
	FlameLightComponent->SetupAttachment(StaticMeshComponent);

	FlameLoopComponent = CreateDefaultSubobject <UAudioComponent>(TEXT("FlameLoopComponent"));
	FlameLoopComponent->SetupAttachment(StaticMeshComponent);
	FlameLoopComponent->bAutoActivate = false;

	LightRadiusComponent = CreateDefaultSubobject<USphereComponent>(TEXT("FlameRadiusComponent"));
	LightRadiusComponent->SetupAttachment(StaticMeshComponent);
	LightRadiusComponent->SetCollisionProfileName(FName("OverlapAllDynamic"));
	LightRadiusComponent->SetGenerateOverlapEvents(false);

	InteractionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionComponent"));
	InteractionComponent->SetupAttachment(StaticMeshComponent);
	InteractionComponent->SetCollisionProfileName(FName("OverlapAllDynamic"));
	InteractionComponent->SetGenerateOverlapEvents(true);

	IgnitionFuelCost = 10.0F;
	bStartIgnited = false;

	bIsTorchIgnited = false;
	TorchIntensity = 0.0F;
}

void ATorchActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsTorchIgnited)
	{
		FlameLightComponent->SetIntensity(FMath::Lerp<float>(FlameLightComponent->Intensity, FlickerIntensity, 0.5F));
	}
}

bool ATorchActor::IsInteractableComponent(UPrimitiveComponent * InOverlappedComponent)
{
	return InteractionComponent == InOverlappedComponent;
}

bool ATorchActor::CanInteract(APyramidMathCharacter* InCharacter)
{
	if (InCharacter)
	{
		return InCharacter->GetTorchFuelAmount() >= IgnitionFuelCost && !bIsTorchIgnited;
	}
	return false;
}

bool ATorchActor::Interact(APyramidMathCharacter* InCharacter)
{
	if (InCharacter)
	{
		if (InCharacter->GetTorchFuelAmount() >= IgnitionFuelCost)
		{
			InCharacter->RemoveTorchFuel(IgnitionFuelCost);
			IgniteTorch();
			return true;
		}
	}
	return false;
}

FText ATorchActor::GetInteractionName()
{
	return NSLOCTEXT("PyramidMath", "INTERACTION_IGNITE_TORCH", "Ignite");
}

bool ATorchActor::IsIgnited()
{
	return bIsTorchIgnited;
}

void ATorchActor::BeginPlay()
{
	Super::BeginPlay();
	TorchIntensity = FlameLightComponent->Intensity;
	FlameLightComponent->SetIntensity(0.0F);
	if (bStartIgnited)
	{
		IgniteTorch();
	}
}

void ATorchActor::FlickerLight()
{
	FlickerIntensity = TorchIntensity + FMath::RandRange(-500.0F, 500.0F);
}

void ATorchActor::IgniteTorch()
{
	FlameParticleComponent->Activate();
	FlameLoopComponent->Play();
	LightRadiusComponent->SetGenerateOverlapEvents(true);
	InteractionComponent->SetGenerateOverlapEvents(false);
	bIsTorchIgnited = true;
	FlickerIntensity = TorchIntensity;
	FlameLightComponent->SetIntensity(TorchIntensity);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_FlickerTimer, this, &ATorchActor::FlickerLight, 0.5F, true);
}