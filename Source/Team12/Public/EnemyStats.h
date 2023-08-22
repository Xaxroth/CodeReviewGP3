// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyStats.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM12_API UEnemyStats : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyStats();

	UPROPERTY(EditAnywhere , Category = "Health")
	float CurrentHealth = 400;


	UFUNCTION(BlueprintCallable)
	void ChangeHealth(float ChangeByThis);

	UFUNCTION(BlueprintCallable)
	float GetHealth(float EnemyHealth);

	void Death();

	bool WasDashedThrough;

	AActor* Encounter;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
