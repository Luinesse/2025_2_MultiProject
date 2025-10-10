// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "DeathZone.h"
#include "MultiController.h"
#include "BaseCharacter.h"

void AMultiGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> DeathZones;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADeathZone::StaticClass(), DeathZones);

	for (AActor* ZoneActor : DeathZones) {
		ADeathZone* DeathZone = Cast<ADeathZone>(ZoneActor);
		if (DeathZone) {
			DeathZone->OnPlayerEntered.AddUObject(this, &AMultiGameModeBase::PlayerDied);
		}
	}
}

void AMultiGameModeBase::PlayerDied(AController* PlayerController)
{
	if (PlayerController == nullptr) {
		return;
	}

	ABaseCharacter* DeadCharacter = Cast<ABaseCharacter>(PlayerController->GetPawn());
	if (DeadCharacter) {
		DeadCharacter->HandleDestruction();
	}

	AMultiController* MyPC = Cast<AMultiController>(PlayerController);
	if (MyPC) {
		MyPC->Client_OnStunned();
	}

	FTimerHandle RespawnTimerHandle;
	FTimerDelegate RespawnDelegate;

	RespawnDelegate.BindUObject(this, &AMultiGameModeBase::RespawnPlayer, PlayerController);
	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, RespawnDelegate, 3.0f, false);
}

void AMultiGameModeBase::RespawnPlayer(AController* PlayerController)
{
	if (PlayerController) {
		AActor* PlayerStart = FindPlayerStart(PlayerController);
		if (PlayerStart == nullptr) {
			PlayerStart = GetWorld()->GetWorldSettings();
		}

		if (PlayerController->GetPawn()) {
			PlayerController->GetPawn()->Destroy();
		}

		RestartPlayerAtPlayerStart(PlayerController, PlayerStart);

		AMultiController* MyPC = Cast<AMultiController>(PlayerController);
		if (MyPC) {
			MyPC->Client_OnUnstunned();
		}
	}
}
