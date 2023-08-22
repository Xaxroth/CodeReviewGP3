// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"

#include "Encounter.h"
#include "EnemyStats.h"
#include "Monster.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	AActor* temp = 	this->GetParentActor();
	AEncounter* Encounter = Cast<AEncounter>(temp);

	if(temp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Parent IS %s"), *temp->GetName());
		Encounter->Spawners.Add(this);
	}

}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AEnemySpawner::SpawnEnemy()
{
	AActor* temp = 	this->GetParentActor();
	AEncounter* Encounter = Cast<AEncounter>(temp);
	
	GEngine -> AddOnScreenDebugMessage(-1 , 1.f , FColor:: Red, "Enemy Spawned");

	int index =	FMath::RandRange(0, PossibleEnemies.Num()-1);
	
	AActor* Current = GetWorld()-> SpawnActor<AActor>(PossibleEnemies[index] , this->GetActorLocation() ,this->GetActorRotation());

	UEnemyStats* Stats =  Cast<UEnemyStats>(Current->GetComponentByClass(UEnemyStats::StaticClass()));

	Stats->Encounter = Encounter;
	
	Encounter->EnemyInstances.Add(Current);


	 AMonster* monster = Cast<AMonster>(Current);

	if(monster)
	{
		monster->SavedEncounter = Encounter;
	}

	 Cast<ATeam12GameMode>(GetWorld()->GetAuthGameMode())->TotalEnemies++;
	
}

