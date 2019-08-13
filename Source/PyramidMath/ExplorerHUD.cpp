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

void AExplorerHUD::ShowAction(const FText & InAction)
{
	if (HUDWidget)
	{
		HUDWidget->EnableAction(InAction);
	}
}

void AExplorerHUD::HideAction()
{
	if (HUDWidget)
	{
		HUDWidget->DisableAction();
	}
}

void AExplorerHUD::BeginPlay()
{
	Super::BeginPlay();
	
	HUDWidget = CreateWidget<UHUDWidget>(GetOwningPlayerController(), HUDWidgetClass);
	HUDWidget->AddToViewport();
}