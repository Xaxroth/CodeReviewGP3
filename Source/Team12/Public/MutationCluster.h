// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MutationActor.h"
#include "MutationCluster.generated.h"

UCLASS()
class TEAM12_API AMutationCluster : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMutationCluster();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mutation Cluster Name")
		FString MutationClusterName;

	UPROPERTY(EditAnywhere, Category = "Mutations To Combine")
		TArray<TSubclassOf<AActor>> Mutations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mutation Data")
		bool RemoveFromMutationPoolOnPickup = true;

	UPROPERTY()
		TArray<TSubclassOf<AActor>> GameModeMutations;

	void Unpack();
	
	
	UPROPERTY(VisibleAnywhere, Category="Player")
	ACharacter* Player;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mutation Cluster Name")
	FString MutationDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mutation Data")
	FString Rarity;


	bool WasChosen;
};
