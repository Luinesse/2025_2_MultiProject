// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnKey.h"
#include "HitTrigger.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Components/BoxComponent.h"
#include "BaseCharacter.h"
#include "Blueprint/UserWidget.h"

// Sets default values
ASpawnKey::ASpawnKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	KeyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMesh"));
	KeyMesh->SetupAttachment(RootComponent);

	KeyMesh->SetIsReplicated(true);

	TriggerPoint = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	TriggerPoint->SetupAttachment(RootComponent);

	TriggerPoint->SetIsReplicated(true);

	bReplicates = true;
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

	TriggerPoint->OnComponentBeginOverlap.AddDynamic(this, &ASpawnKey::OnBeginOverlap);
	TriggerPoint->OnComponentEndOverlap.AddDynamic(this, &ASpawnKey::OnEndOverlap);

	KeyMesh->SetVisibility(false);
}

// Called every frame
void ASpawnKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnKey::ShowHintWidget()
{
	if (HintWidget && DisplayedWidget == nullptr) {
		APlayerController* UserController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		if (UserController) {
			DisplayedWidget = CreateWidget<UUserWidget>(UserController, HintWidget);

			if (DisplayedWidget) {
				DisplayedWidget->AddToViewport();
			}
		}
	}
}

void ASpawnKey::HideHintWidget()
{
	if (DisplayedWidget) {
		DisplayedWidget->RemoveFromParent();
		DisplayedWidget = nullptr;
	}
}

void ASpawnKey::TurnOn(bool isOn)
{
	if (HasAuthority()) {
		if (CurrentCount >= 0 && CurrentCount < MaxCount) {
			if (isOn) {
				CurrentCount++;
			}
		}
	}

	OnRep_CurrentCount();
}

void ASpawnKey::OnRep_CurrentCount()
{
	if (CurrentCount == MaxCount) {
		KeyMesh->SetVisibility(true);
		isActive = true;
	}
	else {
		KeyMesh->SetVisibility(false);
	}
}

void ASpawnKey::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseCharacter* UserCharacter = Cast<ABaseCharacter>(OtherActor);
	if (UserCharacter && UserCharacter->IsLocallyControlled() && isActive) {
		ShowHintWidget();
	}
}

void ASpawnKey::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABaseCharacter* UserCharacter = Cast<ABaseCharacter>(OtherActor);
	if (UserCharacter && UserCharacter->IsLocallyControlled() && isActive) {
		HideHintWidget();
	}
}

void ASpawnKey::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASpawnKey, CurrentCount);
}

