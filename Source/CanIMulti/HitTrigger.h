// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HitTrigger.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHitTriggerChecked, bool);

class UPointLightComponent;

UCLASS()
class CANIMULTI_API AHitTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHitTrigger();

	FOnHitTriggerChecked OnHitTriggerChecked;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* FireHandle;

	UPROPERTY(EditAnywhere, Category = "Light")
	UPointLightComponent* ActiveLight;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_TurnOnLight();

	bool isOn = false;
	bool isActive = false;
};
