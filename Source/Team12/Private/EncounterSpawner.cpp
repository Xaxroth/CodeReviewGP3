// Fill out your copyright notice in the Description page of Project Settings.


#include "EncounterSpawner.h"

// Sets default values
AEncounterSpawner::AEncounterSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEncounterSpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnEncounter();
}

// Called every frame
void AEncounterSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AEncounterSpawner::SpawnEncounter()
{
	int	index =	FMath::RandRange(0, EncounterVariants.Num()-1);
	
	GetWorld()-> SpawnActor<AActor>(EncounterVariants[index] , this->GetActorLocation(),this->GetActorRotation());

}
