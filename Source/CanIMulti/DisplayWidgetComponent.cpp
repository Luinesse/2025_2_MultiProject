// Fill out your copyright notice in the Description page of Project Settings.


#include "DisplayWidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UDisplayWidgetComponent::UDisplayWidgetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDisplayWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDisplayWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDisplayWidgetComponent::ShowHintWidget()
{
	if (HintWidgetClass && DisplayedWidget == nullptr) {
		APlayerController* UserController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		if (UserController) {
			DisplayedWidget = CreateWidget<UUserWidget>(UserController, HintWidgetClass);

			if (DisplayedWidget) {
				DisplayedWidget->AddToViewport();
			}
		}
	}
}

void UDisplayWidgetComponent::HideHintWidget()
{
	if (DisplayedWidget) {
		DisplayedWidget->RemoveFromParent();
		DisplayedWidget = nullptr;
	}
}

