// Fill out your copyright notice in the Description page of Project Settings.


#include "ShowingHint.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "BaseCharacter.h"

// Sets default values
AShowingHint::AShowingHint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerPoint = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Point"));
	TriggerPoint->SetupAttachment(RootComponent);

	TriggerPoint->SetIsReplicated(true);

	bReplicates = true;
}

// Called when the game starts or when spawned
void AShowingHint::BeginPlay()
{
	Super::BeginPlay();
	
	TriggerPoint->OnComponentBeginOverlap.AddDynamic(this, &AShowingHint::OnBeginOverlap);
	TriggerPoint->OnComponentEndOverlap.AddDynamic(this, &AShowingHint::OnEndOverlap);
}

// Called every frame
void AShowingHint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShowingHint::ShowHintWidget()
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

void AShowingHint::HideHintWidget()
{
	if (DisplayedWidget) {
		DisplayedWidget->RemoveFromParent();
		DisplayedWidget = nullptr;
	}
}

void AShowingHint::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseCharacter* UserCharacter = Cast<ABaseCharacter>(OtherActor);
	if (UserCharacter && UserCharacter->IsLocallyControlled()) {
		ShowHintWidget();
	}
}

void AShowingHint::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABaseCharacter* UserCharacter = Cast<ABaseCharacter>(OtherActor);
	if (UserCharacter && UserCharacter->IsLocallyControlled()) {
		HideHintWidget();
	}
}

