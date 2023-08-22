// Copyright Epic Games, Inc. All Rights Reserved.

#include "Team12GameMode.h"
#include "Team12Character.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ATeam12GameMode::ATeam12GameMode(): Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled;
}

void ATeam12GameMode::BeginPlay() 
{
	GameOver = false;
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	// Check if the player character is valid
	if (PlayerCharacter)
	{
		// Get the PlayerStats component attached to the player character
		PlayerStatsComponent = PlayerCharacter->FindComponentByClass<UPlayerStats>();

		// Check if the PlayerStats component is valid
		if (PlayerStatsComponent)
		{

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerStats component not found"));
		}
	}
}

void ATeam12GameMode::GameOverSequence_Implementation()
{
	
}

void ATeam12GameMode::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (!PauseTimer && StartGame) 
	{
		TotalTimeElapsed += deltaTime;


		milliseconds += (10000/ (1/ deltaTime))  ;
		counter++;
		if(counter>= 1/ deltaTime)
		{
			seconds++;
			
			milliseconds =0;
			counter=0;
		}
		int32 DisplayedMilliseconds = milliseconds/100;

		if(seconds>59)
		{
			minutes++;
			seconds=0;
		}
	
	}
}
int ATeam12GameMode::ReturnMinutes()
{
	return minutes;
}
int ATeam12GameMode::ReturnSeconds()
{
	return seconds;
}
int ATeam12GameMode::ReturnMilliseconds()
{
	int32 DisplayedMilliseconds = milliseconds/100; 
	return DisplayedMilliseconds;
	
}

void ATeam12GameMode::StartTimer()
{
	StartGame=true;
}

	