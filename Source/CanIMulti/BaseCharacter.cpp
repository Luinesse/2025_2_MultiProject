// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "AttackComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "GrabComponent.h"

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

	AttackManager = CreateDefaultSubobject<UAttackComponent>(TEXT("AttackComponent"));

	GrabComponent = CreateDefaultSubobject<UGrabComponent>(TEXT("GrabComponent"));
	GrabComponent->SetupAttachment(SpringArmComp);

	PhysicsHandleComponent = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandleComponent"));
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
		Input->BindAction(GrabAction, ETriggerEvent::Triggered, GrabComponent, &UGrabComponent::Grab);
		Input->BindAction(GrabAction, ETriggerEvent::Completed, GrabComponent, &UGrabComponent::Release);
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

	if (AttackManager) {
		AttackManager->Server_Attack();
	}
}