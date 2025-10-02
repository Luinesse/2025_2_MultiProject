// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackComponent.generated.h"

struct FInputActionInstance;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CANIMULTI_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:
	void AttackComplete();

	UFUNCTION(Server, Reliable)
	void Server_AttackComplete();

	UFUNCTION(Server, Reliable)
	void Server_Attack();

	UFUNCTION(Server, Reliable)
	void Server_StartTrace();

	UFUNCTION(Server, Reliable)
	void Server_EndTrace();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Reaction();

	void StartTrace();
	void SphereTracing();
	void EndTrace();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Attack")
	bool IsAttack = false;

	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* HitReaction;

	FTimerHandle AttackHandle;
};
