// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "BaseCharacter.h"
#include "MultiController.h"

// Sets default values for this component's properties
UAttackComponent::UAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	//SetIsReplicated(true);
	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UAttackComponent::AttackComplete()
{
	Server_AttackComplete();
}

void UAttackComponent::Server_StartTrace_Implementation()
{
	GetOwner()->GetWorldTimerManager().SetTimer(AttackHandle, this, &UAttackComponent::SphereTracing, 0.01f, true);
}

void UAttackComponent::Server_Attack_Implementation()
{
	IsAttack = true;
}

void UAttackComponent::Server_AttackComplete_Implementation()
{
	IsAttack = false;
}


// Called every frame
void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAttackComponent::Server_EndTrace_Implementation()
{
	GetOwner()->GetWorldTimerManager().ClearTimer(AttackHandle);
}

void UAttackComponent::Multicast_Reaction_Implementation()
{
	if (HitReaction) {
		ABaseCharacter* MyActor = Cast<ABaseCharacter>(GetOwner());
		if (MyActor) {
			MyActor->PlayAnimMontage(HitReaction);
		}
	}
}

void UAttackComponent::StartTrace()
{
	Server_StartTrace();
}

void UAttackComponent::SphereTracing()
{
	if (!GetOwner()->HasAuthority()) {
		return;
	}

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	ABaseCharacter* MyActor = Cast<ABaseCharacter>(GetOwner());
	USceneComponent* PunchLocate = MyActor->PunchLocationComp;

	FHitResult HitResult;
	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult, PunchLocate->GetComponentLocation(), PunchLocate->GetComponentLocation(),
		FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(15.0f), QueryParams);

	DrawDebugSphere(GetWorld(), PunchLocate->GetComponentLocation(), 15.0f, 5, FColor::Red);

	if (HasHit) {
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 15.0f, 5, FColor::Green, true, 5.0f);
		AActor* HitActor = HitResult.GetActor();
		APawn* HitPawn = Cast<APawn>(HitActor);
		if (HitPawn) {
			AMultiController* VictimPC = Cast<AMultiController>(HitPawn->GetController());
			if (VictimPC) {
				VictimPC->Client_OnStunned();
				FTimerHandle StunHandle;
				GetWorld()->GetTimerManager().SetTimer(
					StunHandle, VictimPC, &AMultiController::Client_OnUnstunned, 1.67f, false);
				UAttackComponent* HitAttackManager = HitActor->FindComponentByClass<UAttackComponent>();
				if (HitAttackManager) {
					HitAttackManager->Multicast_Reaction();
				}
			}
		}
	}
}

void UAttackComponent::EndTrace()
{
	Server_EndTrace();
}

void UAttackComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAttackComponent, IsAttack);
}