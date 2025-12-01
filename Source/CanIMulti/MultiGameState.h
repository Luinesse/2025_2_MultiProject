// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MultiGameState.generated.h"

class UDisplayWidgetComponent;

/**
 * 
 */
UCLASS()
class CANIMULTI_API AMultiGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	AMultiGameState();

public:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ShowClearWidget();

	UPROPERTY(EditAnywhere, Category = "Widget")
	UDisplayWidgetComponent* ClearWidget;
};
