// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MultiGameModeBase.generated.h"

class UDisplayWidgetComponent;

/**
 * 
 */
UCLASS()
class CANIMULTI_API AMultiGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void PlayerDied(AController* PlayerController);

	AMultiGameModeBase();
private:
	void RespawnPlayer(AController* PlayerController);

	FTimerHandle LevelTravelTimer;

	UFUNCTION()
	void ClearFunc();

	void TravelLobby();
};
