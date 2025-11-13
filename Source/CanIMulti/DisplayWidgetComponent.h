// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DisplayWidgetComponent.generated.h"

class UUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CANIMULTI_API UDisplayWidgetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDisplayWidgetComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ShowHintWidget();
	void HideHintWidget();
private:
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> HintWidgetClass;

	UPROPERTY()
	UUserWidget* DisplayedWidget;
};
