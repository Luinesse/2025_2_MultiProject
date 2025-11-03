// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnKey.generated.h"

UCLASS()
class CANIMULTI_API ASpawnKey : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnKey();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* KeyMesh;

	UFUNCTION()
	void TurnOn(bool isOn);

	UPROPERTY()
	int MaxCount;

	UPROPERTY()
	int CurrentCount = 0;
};
