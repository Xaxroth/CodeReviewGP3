// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerStats.h"
#include "GameFramework/Actor.h"
#include "Egg.generated.h"

UCLASS()
class TEAM12_API AEgg : public AActor
{
	GENERATED_BODY()
	
public:	

	AEgg();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Egg Properties")
	float EggHealth = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Egg Properties")
	float SpeedIncreaseAmount = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Egg Properties")
	float HealAmount = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Egg Properties")
	float Duration = 10.f;

	UPROPERTY()
		UPlayerStats* PlayerStatsComponent;

protected:

	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ChangeHealth(float Value);

};
