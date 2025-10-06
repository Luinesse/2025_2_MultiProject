// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values for this component's properties
UGrabComponent::UGrabComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UGrabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetOwner() && GetOwner()->HasAuthority()) {
		UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
		if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent()) {
			FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
			PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
		}
	}
}

void UGrabComponent::Grab()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr || !OwnerPawn->IsLocallyControlled()) {
		return;
	}

	FHitResult HitResult;
	if (GetGrabbableInReach(HitResult)) {
		Server_Grab(HitResult.GetComponent());
	}
}

void UGrabComponent::Server_Grab_Implementation(UPrimitiveComponent* HitComp)
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if (PhysicsHandle == nullptr || HitComp == nullptr) {
		return;
	}

	HitComp->WakeAllRigidBodies();
	HitComp->SetSimulatePhysics(true);
	PhysicsHandle->GrabComponentAtLocationWithRotation(HitComp, NAME_None, HitComp->GetComponentLocation(), HitComp->GetComponentRotation());
}

void UGrabComponent::Release()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr || !OwnerPawn->IsLocallyControlled()) {
		return;
	}

	Server_Release();
}

void UGrabComponent::Server_Release_Implementation()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if (PhysicsHandle == nullptr || PhysicsHandle->GetGrabbedComponent() == nullptr) {
		return;
	}

	UPrimitiveComponent* GrabbedComp = PhysicsHandle->GetGrabbedComponent();
	PhysicsHandle->ReleaseComponent();
}

UPhysicsHandleComponent* UGrabComponent::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (!PhysicsHandle) {
		return nullptr;
	}

	return PhysicsHandle;
}

bool UGrabComponent::GetGrabbableInReach(FHitResult& HitResult) const
{
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	DrawDebugSphere(GetWorld(), End, GrabRadius, 10, FColor::Red, true, 5.0f);
	bool HasHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel1, Sphere);

	return HasHit;
}
