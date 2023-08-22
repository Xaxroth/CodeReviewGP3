// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlayerStats.h"
#include "GameFramework/GameModeBase.h"
#include "Team12GameMode.generated.h"

UCLASS(minimalapi)
class ATeam12GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATeam12GameMode();

	UPROPERTY(VisibleAnywhere, Category = "Time")
		float TotalTimeElapsed;

	UPROPERTY(EditAnywhere, Category = "My Actors")
		TArray<TSubclassOf<AActor>> Mutations;

	UPROPERTY()
		UPlayerStats* PlayerStatsComponent;

	int32 milliseconds;
	int32 seconds;
	int32 minutes;

	UFUNCTION(BlueprintCallable)
	int ReturnMinutes();
	UFUNCTION(BlueprintCallable)
	int ReturnSeconds();
	UFUNCTION(BlueprintCallable)
	int ReturnMilliseconds();

	UPROPERTY(EditAnywhere, Category="Saved Data")
	TArray<float> RankingsInSeconds = {120, 150, 180, 210, 240, 300};  ;//done
	
	int counter;
	
	bool PauseTimer;
	bool GameOver;
	bool MenuActive;


	UFUNCTION(BlueprintCallable)
	void StartTimer();
	
	int kills;
	int dashes;
	int shotsTaken;
	int HPLost;
	UPROPERTY(VisibleAnywhere, Category = "Stored Data")
	int EggsDestroyed;
	FString LastEncounterTime;

	UPROPERTY(VisibleAnywhere, Category = "Stored Data")
	int TotalEggs;

	UPROPERTY(VisibleAnywhere, Category = "Stored Data")
	int TotalEnemies;
	
public:

	virtual void Tick(float deltaTime) override;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, Category = "MyEvents")
		void GameOverSequence();
	virtual void GameOverSequence_Implementation();



private:
	bool StartGame;
};



