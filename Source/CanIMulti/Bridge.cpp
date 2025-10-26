// Fill out your copyright notice in the Description page of Project Settings.


#include "Bridge.h"

// Sets default values
ABridge::ABridge()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Bridge = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bridge"));
	Bridge->SetupAttachment(RootComponent);

	CollisionLocation = CreateDefaultSubobject<USceneComponent>(TEXT("CollisionLocation"));
	CollisionLocation->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABridge::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABridge::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

