// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GrabComponent.generated.h"

class UPhysicsHandleComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CANIMULTI_API UGrabComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Grab();

	UFUNCTION(Server, Reliable)
	void Server_Grab(UPrimitiveComponent* HitComp);

	void Release();

	UFUNCTION(Server, Reliable)
	void Server_Release();

private:
	UPROPERTY(EditAnywhere)
	float MaxGrabDistance = 400;

	UPROPERTY(EditAnywhere)
	float GrabRadius = 100;

	UPROPERTY(EditAnywhere)
	float HoldDistance;

	UPhysicsHandleComponent* GetPhysicsHandle() const;
	bool GetGrabbableInReach(FHitResult& HitResult) const;
};
