// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnKey.generated.h"

class UBoxComponent;
class UUserWidget;
class UDisplayWidgetComponent;

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

	UPROPERTY(EditAnywhere, Category = "Hint")
	UBoxComponent* TriggerPoint;

	UPROPERTY(EditAnywhere, Category = "Hint")
	UDisplayWidgetComponent* HintDraw;

	UFUNCTION()
	void TurnOn(bool isOn);

	UPROPERTY()
	int MaxCount;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentCount)
	int CurrentCount = 0;

	UFUNCTION()
	void OnRep_CurrentCount();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	bool isActive = false;

};
