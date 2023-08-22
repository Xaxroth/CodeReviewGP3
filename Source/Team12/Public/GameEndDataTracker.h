// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Team12/Team12GameMode.h"
#include "GameEndDataTracker.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM12_API UGameEndDataTracker : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGameEndDataTracker();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, Category="Saved Data")
	FString FinalTime;//done
	UPROPERTY(VisibleAnywhere, Category="Saved Data")
	float FinalTimeSeconds;//done
	UPROPERTY(VisibleAnywhere, Category="Saved Data")
	TArray<FString> TimeForEachSection;//done
	TArray<float> RankingsInSeconds ;  //done
	UPROPERTY(EditAnywhere, Category="Saved Data")
	FString Ranking;//done

	UFUNCTION(BlueprintCallable)
	void GameEnd(FString& Time , FString& TimeRanking ,FString& DashesMade ,FString& DamageTaken , FString& AmountOfKills , FString& ShotsTaken , TArray<FString>& TimeForEachEncounter , FString& DestroyedEggs);
	ATeam12GameMode* GameMode;
	
};


