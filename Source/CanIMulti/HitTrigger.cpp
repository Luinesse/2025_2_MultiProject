// Fill out your copyright notice in the Description page of Project Settings.


#include "HitTrigger.h"
#include "BaseCharacter.h"

// Sets default values
AHitTrigger::AHitTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FireHandle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FireHandle"));
	FireHandle->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AHitTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	FireHandle->OnComponentHit.AddDynamic(this, &AHitTrigger::OnHit);
}

// Called every frame
void AHitTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHitTrigger::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (HasAuthority()) {
		ABaseCharacter* PlayerCharacter = Cast<ABaseCharacter>(OtherActor);

		if (PlayerCharacter) {
			isOn = true;
			OnHitTriggerChecked.Broadcast(isOn);
		}
	}
}