// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiGameState.h"
#include "DisplayWidgetComponent.h"

AMultiGameState::AMultiGameState()
{
	ClearWidget = CreateDefaultSubobject<UDisplayWidgetComponent>(TEXT("WidgetComp"));
}

void AMultiGameState::Multicast_ShowClearWidget_Implementation()
{
	if (ClearWidget) {
		ClearWidget->ShowHintWidget();
	}
}
