// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathZone.h"
#include "Components/StaticMeshComponent.h"
#include "BaseCharacter.h"

// Sets default values
ADeathZone::ADeathZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	RootComponent = MeshComp;

	MeshComp->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void ADeathZone::BeginPlay()
{
	Super::BeginPlay();
	
	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &ADeathZone::OnOverlapBegin);
}

// Called every frame
void ADeathZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADeathZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority()) {
		ABaseCharacter* PlayerCharacter = Cast<ABaseCharacter>(OtherActor);

		if (PlayerCharacter) {
			OnPlayerEntered.Broadcast(PlayerCharacter->GetController());
		}
	}
}

