// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MultiController.generated.h"

/**
 * 
 */
UCLASS()
class CANIMULTI_API AMultiController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(Client, Reliable)
	void Client_OnStunned();

	UFUNCTION(Client, Reliable)
	void Client_OnUnstunned();
};
