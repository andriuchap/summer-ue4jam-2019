// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootablePhysicalMaterial.h"
/*#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"*/
#include "ImpactDecalActor.h"
#include "Kismet/GameplayStatics.h"

UShootablePhysicalMaterial::UShootablePhysicalMaterial(const FObjectInitializer& ObjInitializer)
{

}

void UShootablePhysicalMaterial::ShowImpact(const FHitResult & Hit)
{
	AActor* WorldActor = Hit.GetActor();
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(WorldActor, ImpactSound, Hit.Location);
	}
	if (ImpactParticle)
	{
		FTransform Transform;
		Transform.SetLocation(Hit.Location);
		Transform.SetRotation(Hit.ImpactNormal.ToOrientationRotator().Quaternion());
		UGameplayStatics::SpawnEmitterAtLocation(WorldActor->GetWorld(), ImpactParticle, Transform);
	}
	if (ImpactDecalClass)
	{
		UWorld* World = WorldActor->GetWorld();
		if (World)
		{
			FTransform Transform;
			Transform.SetLocation(Hit.Location);
			Transform.SetRotation(Hit.Normal.ToOrientationRotator().Quaternion());
			World->SpawnActor<AImpactDecalActor>(ImpactDecalClass, Transform);
		}
	}
}
