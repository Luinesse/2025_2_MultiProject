// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnBlockKey.generated.h"

class ASequenceBlock;

UCLASS()
class CANIMULTI_API ASpawnBlockKey : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnBlockKey();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* ClearKey;

	UPROPERTY(EditAnywhere, Category = "Gimmick")
	FName KeyTagName;

	UFUNCTION()
	void CheckNum(int num);

	UPROPERTY(EditAnywhere, Category = "Gimmick")
	TArray<int> resultArray;

	UPROPERTY()
	TArray<int> inputArray;

	UPROPERTY(ReplicatedUsing = OnRep_CheckResult)
	bool IsGimmickClear;

	UPROPERTY()
	TArray<ASequenceBlock*> ManagedBlocks;

	UFUNCTION()
	void OnRep_CheckResult();

	UFUNCTION()
	void OnKeyOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void ResetGimmick();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
