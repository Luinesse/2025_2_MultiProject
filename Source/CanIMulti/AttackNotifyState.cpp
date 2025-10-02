// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackNotifyState.h"
#include "BaseCharacter.h"
#include "AttackComponent.h"

void UAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	ABaseCharacter* UserPlayer = Cast<ABaseCharacter>(MeshComp->GetOwner());

	if (UserPlayer) {
		UserPlayer->FindComponentByClass<UAttackComponent>()->StartTrace();
	}
}

void UAttackNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	ABaseCharacter* UserPlayer = Cast<ABaseCharacter>(MeshComp->GetOwner());

	if (UserPlayer) {
		UserPlayer->FindComponentByClass<UAttackComponent>()->EndTrace();
	}
}
