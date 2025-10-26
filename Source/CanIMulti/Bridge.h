// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bridge.generated.h"

UCLASS()
class CANIMULTI_API ABridge : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABridge();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* Bridge;
	UPROPERTY(EditAnywhere, Category = "Location")
	USceneComponent* CollisionLocation;
};
