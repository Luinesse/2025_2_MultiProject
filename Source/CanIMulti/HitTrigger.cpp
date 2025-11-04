// Fill out your copyright notice in the Description page of Project Settings.


#include "HitTrigger.h"
#include "BaseCharacter.h"
#include "Components/PointLightComponent.h"

// Sets default values
AHitTrigger::AHitTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FireHandle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FireHandle"));
	FireHandle->SetupAttachment(RootComponent);

	ActiveLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("ActiveLight"));
	ActiveLight->SetupAttachment(FireHandle);

	ActiveLight->SetIntensity(0.0f);

	bReplicates = true;
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
	if (HasAuthority() && !isActive) {
		ABaseCharacter* PlayerCharacter = Cast<ABaseCharacter>(OtherActor);

		if (PlayerCharacter) {
			isOn = true;
			isActive = true;

			Multicast_TurnOnLight();

			OnHitTriggerChecked.Broadcast(isOn);
		}
	}
}

void AHitTrigger::Multicast_TurnOnLight_Implementation()
{
	ActiveLight->SetIntensity(5000.0f);
}
