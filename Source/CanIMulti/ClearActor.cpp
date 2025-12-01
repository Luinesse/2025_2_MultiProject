// Fill out your copyright notice in the Description page of Project Settings.


#include "ClearActor.h"
#include "Components/BoxComponent.h"
#include "DisplayWidgetComponent.h"

// Sets default values
AClearActor::AClearActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TriggerPoint = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerPoint"));
	TriggerPoint->SetupAttachment(RootComponent);

	TriggerPoint->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

}

// Called when the game starts or when spawned
void AClearActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority()) {
		TriggerPoint->OnComponentBeginOverlap.AddDynamic(this, &AClearActor::OnBeginOverlap);
	}
}

// Called every frame
void AClearActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AClearActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority())	return;

	OnLevelClear.Broadcast();
	TriggerPoint->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
