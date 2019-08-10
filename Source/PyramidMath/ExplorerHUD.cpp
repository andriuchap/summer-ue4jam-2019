// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplorerHUD.h"
#include "HUDWidget.h"

AExplorerHUD::AExplorerHUD(const FObjectInitializer &ObjInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	HUDWidgetClass = UHUDWidget::StaticClass();
	HUDWidget = nullptr;
}

void AExplorerHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AExplorerHUD::BeginPlay()
{
	Super::BeginPlay();
	
	HUDWidget = CreateWidget<UHUDWidget>(GetOwningPlayerController(), HUDWidgetClass);
	HUDWidget->AddToViewport();
}