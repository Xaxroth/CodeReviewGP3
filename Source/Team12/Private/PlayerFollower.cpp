// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFollower.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Team12/Team12Character.h"

// Sets default values
APlayerFollower::APlayerFollower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Dash Hit Sphere"));
	sphere -> SetSphereRadius(32);
	sphere -> SetCollisionProfileName("Trigger");

}

// Called when the game starts or when spawned
void APlayerFollower::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void APlayerFollower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	this->SetActorTransform(player->GetActorTransform());
}

