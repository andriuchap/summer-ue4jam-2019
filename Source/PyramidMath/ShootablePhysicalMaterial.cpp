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
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, Hit.Location);
	}
	if (ImpactParticle)
	{
		FTransform Transform;
		Transform.SetLocation(Hit.Location);
		Transform.SetRotation(Hit.ImpactNormal.ToOrientationRotator().Quaternion());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, Transform);
	}
	if (ImpactDecalClass)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			FTransform Transform;
			Transform.SetLocation(Hit.Location);
			Transform.SetRotation(Hit.Normal.ToOrientationRotator().Quaternion());
			World->SpawnActor<AImpactDecalActor>(ImpactDecalClass, Transform);
		}
	}
}
