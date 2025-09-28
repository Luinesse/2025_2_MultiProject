// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "MultiController.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);

	USkeletalMeshComponent* CharacterMesh = GetMesh();
	PunchLocationComp = CreateDefaultSubobject<USceneComponent>(TEXT("PunchLocate"));
	PunchLocationComp->SetupAttachment(CharacterMesh, TEXT("Punch"));
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController != nullptr) {
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		if (Subsystem != nullptr) {
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (Input != nullptr) {
		Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		Input->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Move);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Look);
		Input->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Attack);
	}
}

void ABaseCharacter::OpenLobby()
{
	UWorld* World = GetWorld();
	if (World) {
		World->ServerTravel(TEXT("/Game/Level/Main?listen"));
		isServer = true;
	}
}

void ABaseCharacter::CallOpenLevel(const FString& Address)
{
	if (isServer)	return;

	UGameplayStatics::OpenLevel(this, FName(*Address));
}

void ABaseCharacter::CallClinetTravel(const FString& Address)
{
	if (isServer)	return;

	APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
	if (PlayerController) {
		PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	}
}

void ABaseCharacter::Move(const FInputActionInstance& Instance)
{
	FVector2D MovementVector = Instance.GetValue().Get<FVector2D>();

	if (Controller != nullptr) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector FowardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(FowardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ABaseCharacter::Look(const FInputActionInstance& Instance)
{
	FVector2D LookVector = Instance.GetValue().Get<FVector2D>();

	if (Controller != nullptr) {
		AddControllerYawInput(LookVector.X);
		AddControllerPitchInput(LookVector.Y);
	}
}

void ABaseCharacter::Attack(const FInputActionInstance& Instance)
{
	if (!IsLocallyControlled()) {
		return;
	}

	if (!IsAttack) {
		Server_Attack();
	}
}

void ABaseCharacter::Server_AttackComplete_Implementation()
{
	IsAttack = false;
}

void ABaseCharacter::AttackComplete()
{
	Server_AttackComplete();
}

void ABaseCharacter::Server_Attack_Implementation()
{
	IsAttack = true;
}

void ABaseCharacter::Server_StartTrace_Implementation()
{
	GetWorldTimerManager().SetTimer(AttackHandle, this, &ABaseCharacter::SphereTracing, 0.01f, true);
}

void ABaseCharacter::Server_EndTrace_Implementation()
{
	GetWorldTimerManager().ClearTimer(AttackHandle);
}

void ABaseCharacter::Multicast_Reaction_Implementation()
{
	if (HitReaction) {
		PlayAnimMontage(HitReaction);
	}
}

void ABaseCharacter::StartTrace()
{
	Server_StartTrace();
}

void ABaseCharacter::SphereTracing()
{
	if (!HasAuthority()) {
		return;
	}

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	FHitResult HitResult;
	bool HasHit = GetWorld()->SweepSingleByChannel(HitResult, PunchLocationComp->GetComponentLocation(), PunchLocationComp->GetComponentLocation(), FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(15.0f), QueryParams);
	DrawDebugSphere(GetWorld(), PunchLocationComp->GetComponentLocation(), 15.0f, 5, FColor::Red);

	if (HasHit) {
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 15.0f, 5, FColor::Green, true, 5.0f);
		AActor* HitActor = HitResult.GetActor();
		APawn* HitPawn = Cast<APawn>(HitActor);
		if (HitPawn) {
			AMultiController* VictimPC = Cast<AMultiController>(HitPawn->GetController());
			if (VictimPC) {
				VictimPC->Client_OnStunned();
				FTimerHandle StunHandle;
				GetWorld()->GetTimerManager().SetTimer(StunHandle, VictimPC, &AMultiController::Client_OnUnstunned, 1.67f, false);
				ABaseCharacter* ReactActor = Cast<ABaseCharacter>(HitActor);
				if (ReactActor) {
					ReactActor->Multicast_Reaction();
				}
			}
		}
	}
}

void ABaseCharacter::EndTrace()
{
	Server_EndTrace();
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, IsAttack);
}
