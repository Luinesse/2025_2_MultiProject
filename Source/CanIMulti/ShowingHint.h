// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShowingHint.generated.h"

class UBoxComponent;
class UUserWidget;
class UDisplayWidgetComponent;

UCLASS()
class CANIMULTI_API AShowingHint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShowingHint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "Widget")
	UBoxComponent* TriggerPoint;

	UPROPERTY(EditAnywhere, Category = "Widget")
	UDisplayWidgetComponent* HintDraw;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
