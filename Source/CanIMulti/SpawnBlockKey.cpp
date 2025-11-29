// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnBlockKey.h"
#include "SequenceBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASpawnBlockKey::ASpawnBlockKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ClearKey = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClearKey"));
	ClearKey->SetupAttachment(RootComponent);

	ClearKey->SetIsReplicated(true);

	ClearKey->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ClearKey->SetVisibility(false);
	ClearKey->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ClearKey->SetGenerateOverlapEvents(true);

	KeyTagName = TEXT("Clear");

	bReplicates = true;
}

// Called when the game starts or when spawned
void ASpawnBlockKey::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<AActor*> SequenceBlocks;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASequenceBlock::StaticClass(), SequenceBlocks);

	for (AActor* SeqBlock : SequenceBlocks) {
		ASequenceBlock* Block = Cast<ASequenceBlock>(SeqBlock);
		if (Block) {
			Block->OnOverlapChecked.AddUObject(this, &ASpawnBlockKey::CheckNum);
			ManagedBlocks.Add(Block);
		}
	}

	if (HasAuthority()) {
		ClearKey->OnComponentBeginOverlap.AddDynamic(this, &ASpawnBlockKey::OnKeyOverlap);
	}
}

// Called every frame
void ASpawnBlockKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnBlockKey::CheckNum(int num)
{
	if (HasAuthority()) {

		if (IsGimmickClear) return;
		inputArray.Add(num);

		if (inputArray.Num() > resultArray.Num()) {
			ResetGimmick();
			return;
		}

		if (resultArray[inputArray.Num() - 1] != num) {
			ResetGimmick();
			return;
		}

		if (inputArray.Num() == resultArray.Num()) {
			IsGimmickClear = true;
			OnRep_CheckResult();
		}
	}
}

void ASpawnBlockKey::OnRep_CheckResult()
{
	if (IsGimmickClear) {
		ClearKey->SetVisibility(true);
		ClearKey->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void ASpawnBlockKey::OnKeyOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority())	return;
	if (!IsGimmickClear)	return;

	APawn* OverlappedPawn = Cast<APawn>(OtherActor);
	if (!OverlappedPawn || !OverlappedPawn->IsPlayerControlled())	return;

	for (auto Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator) {
		APlayerController* PC = Iterator->Get();
		if (PC) {
			APawn* MyPawn = PC->GetPawn();
			if (MyPawn) {
				UE_LOG(LogTemp, Display, TEXT("Tag Set"));
				MyPawn->Tags.AddUnique(KeyTagName);
			}
		}
	}

	Destroy();
}

void ASpawnBlockKey::ResetGimmick()
{
	inputArray.Empty();
	for (ASequenceBlock* Block : ManagedBlocks) {
		if (Block) {
			Block->ResetBlock();
		}
	}
}

void ASpawnBlockKey::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASpawnBlockKey, IsGimmickClear);
}

