// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyStats.h"
#include "Monster.h"
#include "Encounter.h"
#include "Monster.h"
#include "Windows/LiveCoding/Private/External/LC_ClientStartupThread.h"

class AMonster;
// Sets default values for this component's properties
UEnemyStats::UEnemyStats()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyStats::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEnemyStats::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEnemyStats::ChangeHealth(float ChangeByThis)
{
	CurrentHealth += ChangeByThis;

	if(CurrentHealth<=0)
	{
		Death();

		AMonster* MonsterScript = Cast<AMonster>(GetOwner());
		if (MonsterScript && MonsterScript->State != MonsterState::Dead)
		{
			MonsterScript->State = MonsterState::Dead;
		}
		
	}
}

float UEnemyStats::GetHealth(float EnemyHealth) 
{
	return CurrentHealth;
}


void UEnemyStats::Death()
{
	AEncounter* temp = Cast<AEncounter>(Encounter);

	ATeam12GameMode* GameMode =  Cast<ATeam12GameMode>( GetWorld()->GetAuthGameMode());
	
	if(temp){temp->EnemyInstances.Remove(GetOwner());}

	AMonster* monster = Cast<AMonster>(this->GetOwner());

	if(monster)
	{
		if(Encounter){
		monster->SavedEncounter->UpdateEnemyList();
		}
		monster->MonsterIsDead();
	}


	if(monster &&  monster->State!=MonsterState::Dead)
	{
	GameMode->kills++;	
	}
}



