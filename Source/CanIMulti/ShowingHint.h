// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WidgetInterface.h"
#include "ShowingHint.generated.h"

class UBoxComponent;
class UUserWidget;

UCLASS()
class CANIMULTI_API AShowingHint : public AActor, public IWidgetInterface
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

	virtual void ShowHintWidget() override;
	virtual void HideHintWidget() override;

private:
	UPROPERTY(EditAnywhere, Category = "Widget")
	UBoxComponent* TriggerPoint;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> HintWidget;

	UPROPERTY()
	UUserWidget* DisplayedWidget;
};
