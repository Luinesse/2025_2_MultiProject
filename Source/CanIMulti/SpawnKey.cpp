// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnKey.h"
#include "HitTrigger.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpawnKey::ASpawnKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	KeyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMesh"));
	KeyMesh->SetupAttachment(RootComponent);

	KeyMesh->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void ASpawnKey::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<AActor*> HitTriggers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHitTrigger::StaticClass(), HitTriggers);

	MaxCount = HitTriggers.Num();

	for (AActor* HitTrigger : HitTriggers) {
		AHitTrigger* Trigger = Cast<AHitTrigger>(HitTrigger);
		if (Trigger) {
			Trigger->OnHitTriggerChecked.AddUObject(this, &ASpawnKey::TurnOn);
		}
	}

	KeyMesh->SetVisibility(false);
}

// Called every frame
void ASpawnKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnKey::TurnOn(bool isOn)
{
	if (CurrentCount >= 0 && CurrentCount < MaxCount) {
		if (isOn) {
			CurrentCount++;
		}
		else {
			CurrentCount--;
		}
	}

	if (CurrentCount == MaxCount) {
		KeyMesh->SetVisibility(true);
	}
	else {
		KeyMesh->SetVisibility(false);
	}
}

