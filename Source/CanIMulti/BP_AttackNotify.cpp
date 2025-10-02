// Fill out your copyright notice in the Description page of Project Settings.


#include "BP_AttackNotify.h"
#include "BaseCharacter.h"
#include "AttackComponent.h"

void UBP_AttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp) {
		ABaseCharacter* Player = Cast<ABaseCharacter>(MeshComp->GetOwner());

		if (Player) {
			Player->FindComponentByClass<UAttackComponent>()->AttackComplete();
		}
	}
}
