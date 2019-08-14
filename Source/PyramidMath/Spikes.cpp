// Fill out your copyright notice in the Description page of Project Settings.


#include "Spikes.h"
#include "Components/StaticMeshComponent.h"
#include "PyramidMathCharacter.h"

ASpikes::ASpikes(const FObjectInitializer& ObjInitializer)
{
	GetStaticMeshComponent()->SetGenerateOverlapEvents(true);
	GetStaticMeshComponent()->SetCollisionProfileName(FName("OverlapAllDynamic"));
}

void ASpikes::BeginPlay()
{
	Super::BeginPlay();
	GetStaticMeshComponent()->OnComponentBeginOverlap.AddDynamic(this, &ASpikes::SpikesBeginOverlap);
}

void ASpikes::SpikesBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Cucoo!"));
	APyramidMathCharacter* Character = Cast<APyramidMathCharacter>(OtherActor);
	if(Character)
	{
		FVector DamageDirection = Character->GetActorLocation() - GetActorLocation();
		DamageDirection.Normalize();
		Character->DealDamage(1, DamageDirection);
	}
}
