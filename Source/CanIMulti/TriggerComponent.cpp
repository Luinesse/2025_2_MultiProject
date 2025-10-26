// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"
#include "Mover.h"

UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* FindActor = GetAcceptableActor();

	if (FindActor) {
		Mover->setShouldMove(true);
		UE_LOG(LogTemp, Display, TEXT("Check !!"));
	}
	else {
		Mover->setShouldMove(false);
		UE_LOG(LogTemp, Display, TEXT("UnCheck !!"));
	}
}

void UTriggerComponent::SetMover(UMover* NewMover)
{
	Mover = NewMover;
}

AActor* UTriggerComponent::GetAcceptableActor() const
{
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);

	for (AActor* Actor : Actors) {
		if (Actor->ActorHasTag(AcceptableActorTag)) {
			return Actor;
		}
	}

	return nullptr;
}
