// Fill out your copyright notice in the Description page of Project Settings.


#include "ScarabDeathCounter.h"
#include "Scarab.h"
#include "DoorActor.h"

AScarabDeathCounter::AScarabDeathCounter(const FObjectInitializer &ObjInitializer)
{
	Scarabs = TArray<AScarab*>();
	Door = nullptr;

	ScarabsRemain = 0;
}

void AScarabDeathCounter::BeginPlay()
{
	Super::BeginPlay();
	for (int i = 0; i < Scarabs.Num(); i++)
	{
		Scarabs[i]->OnScarabDied.AddUFunction(this, FName("OnScarabDied"));
	}
	ScarabsRemain = Scarabs.Num();
}

void AScarabDeathCounter::OnScarabDied()
{
	ScarabsRemain--;
	if (ScarabsRemain == 0 && Door)
	{
		Door->OpenDoor();
	}
}
