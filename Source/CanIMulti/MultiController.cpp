// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiController.h"

void AMultiController::Client_OnStunned_Implementation()
{
	if (GetPawn()) {
		GetPawn()->DisableInput(this);
	}
}

void AMultiController::Client_OnUnstunned_Implementation()
{
	if (GetPawn()) {
		GetPawn()->EnableInput(this);
	}
}
