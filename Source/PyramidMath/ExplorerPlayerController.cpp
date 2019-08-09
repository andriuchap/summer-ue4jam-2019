// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplorerPlayerController.h"
#include "ExplorerCursorWidget.h"

AExplorerPlayerController::AExplorerPlayerController(const FObjectInitializer &ObjInitializer)
{
	bShowMouseCursor = true;
}

void AExplorerPlayerController::BeginPlay()
{
	Super::BeginPlay();
	CursorWidget = CreateWidget<UExplorerCursorWidget>(this, CursorWidgetClass);
	SetMouseCursorWidget(EMouseCursor::Default, CursorWidget);
}