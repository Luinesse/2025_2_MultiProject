// Fill out your copyright notice in the Description page of Project Settings.


#include "SequenceBlock.h"
#include "Components/BoxComponent.h"
#include "BaseCharacter.h"

// Sets default values
ASequenceBlock::ASequenceBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Block = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Block"));
	Block->SetupAttachment(RootComponent);

	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	OverlapBox->SetupAttachment(Block);
}

// Called when the game starts or when spawned
void ASequenceBlock::BeginPlay()
{
	Super::BeginPlay();
	
	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ASequenceBlock::OnOverlap);
}

// Called every frame
void ASequenceBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASequenceBlock::ResetBlock()
{
	if (HasAuthority()) {
		isActive = false;
	}
}

void ASequenceBlock::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority() && !isActive) {
		ABaseCharacter* isCharacter = Cast<ABaseCharacter>(OtherActor);
		if (isCharacter) {
			isActive = true;
			OnOverlapChecked.Broadcast(myNumber);
		}
	}
}

