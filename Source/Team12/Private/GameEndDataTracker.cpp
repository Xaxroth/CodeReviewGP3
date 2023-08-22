// Fill out your copyright notice in the Description page of Project Settings.


#include "GameEndDataTracker.h"

// Sets default values for this component's properties
UGameEndDataTracker::UGameEndDataTracker()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGameEndDataTracker::BeginPlay()
{
	Super::BeginPlay();

	// ...

	GameMode = Cast<ATeam12GameMode>(GetWorld()->GetAuthGameMode());
	
}


// Called every frame
void UGameEndDataTracker::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	if(!TimeForEachSection.Contains(GameMode->LastEncounterTime))
	{
		TimeForEachSection.Add(GameMode->LastEncounterTime);
	}

	if(TimeForEachSection.Contains(""))
	{
		TimeForEachSection.Remove("");
	}
	
	// Air time
	/*int TimeSpentMidAirSeconds =TimeSpentMidAir/1000;
	TimeSpentMidAir= TimeSpentMidAir/1000;
	int TimeSpentMidAirMinutes = TimeSpentMidAir/60;
	TimeSpentMidAirSeconds = TimeSpentMidAirSeconds/60;

	AirTimeVisible= "";
	
	AirTimeVisible.AppendInt(TimeSpentMidAirMinutes);
	AirTimeVisible.Append(":");
	AirTimeVisible.AppendInt(TimeSpentMidAirSeconds);
	AirTimeVisible.Append(":");
	AirTimeVisible.AppendInt(TimeSpentMidAir);*/
	
}

void UGameEndDataTracker::GameEnd( FString& Time , FString& TimeRanking ,FString& DashesMade ,FString& DamageTaken , FString& AmountOfKills , FString& ShotsTaken , TArray<FString>& TimeForEachEncounter , FString& DestroyedEggs)
{
	 FinalTimeSeconds = GameMode->ReturnSeconds() +GameMode->ReturnMinutes()*60;

	RankingsInSeconds = GameMode->RankingsInSeconds;
	
	if(FinalTimeSeconds < RankingsInSeconds[0])
	{
		Ranking = "S";
	}else if(FinalTimeSeconds < RankingsInSeconds[1])
	{
		Ranking = "A";
	}else if(FinalTimeSeconds < RankingsInSeconds[2])
	{
		Ranking = "B";
	}else if(FinalTimeSeconds < RankingsInSeconds[3])
	{
		Ranking = "C";
	}else if(FinalTimeSeconds < RankingsInSeconds[4])
	{
		Ranking = "D";
	}else if(FinalTimeSeconds < RankingsInSeconds[5])
	{
		Ranking = "E";
	}else if(FinalTimeSeconds < RankingsInSeconds[6])
	{
		Ranking = "F";
	}else
	{
		Ranking = "F-";
	}


	
	// return all the other stuff

	FinalTime.AppendInt(GameMode->ReturnMinutes()) ;
	FinalTime.Append(":");
	FinalTime.AppendInt(GameMode->ReturnSeconds()) ;
	FinalTime.Append(":");
	FinalTime.AppendInt(GameMode->ReturnMilliseconds()) ;

	Time= FinalTime;
	
	TimeRanking =Ranking;
	DashesMade.AppendInt(GameMode->dashes);
	DamageTaken.AppendInt(-GameMode->HPLost);
	ShotsTaken.AppendInt(GameMode->shotsTaken);
	TimeForEachEncounter=  TimeForEachSection;

	AmountOfKills.AppendInt(GameMode->kills);
	AmountOfKills.Append("/");
	AmountOfKills.AppendInt(GameMode->TotalEnemies);
	
	DestroyedEggs.AppendInt(GameMode->EggsDestroyed);
	DestroyedEggs.Append("/");
	DestroyedEggs.AppendInt(GameMode->TotalEggs);
}
