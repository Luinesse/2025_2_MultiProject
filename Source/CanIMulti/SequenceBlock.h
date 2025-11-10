// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SequenceBlock.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnOverlapChecked, int);

class UBoxComponent;

UCLASS()
class CANIMULTI_API ASequenceBlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASequenceBlock();

	FOnOverlapChecked OnOverlapChecked;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void ResetBlock();
private:
	UPROPERTY(EditAnywhere, Category = "Number")
	int myNumber;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* Block;

	UPROPERTY(EditAnywhere)
	UBoxComponent* OverlapBox;

	bool isActive = false;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
