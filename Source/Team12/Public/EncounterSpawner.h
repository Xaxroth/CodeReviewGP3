// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Encounter.h"
#include "GameFramework/Actor.h"
#include "EncounterSpawner.generated.h"

UCLASS()
class TEAM12_API AEncounterSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEncounterSpawner();


	UPROPERTY(EditAnywhere, Category = "Encounter Variants")
	TArray<TSubclassOf<AEncounter>>EncounterVariants;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnEncounter();
};
