// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MutationManager.generated.h"

UCLASS()
class TEAM12_API AMutationManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMutationManager();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mutations")
		TArray<UActorComponent*> AllMutations;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
