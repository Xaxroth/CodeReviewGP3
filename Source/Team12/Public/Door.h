// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class TEAM12_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OpenDoor();
	void CloseDoor();

private:
	
	UPROPERTY(VisibleAnywhere, Category ="Parent")
	AActor* parent;


	UPROPERTY(EditAnywhere, Category ="Door")
	bool isEntrance;


	// Changing position related things
	UPROPERTY(EditAnywhere, Category ="Door")
	float ClosedOffSet;
	UPROPERTY(EditAnywhere, Category ="Door")
	FVector CurrentPosition;
	FVector OpenPosition;

	UPROPERTY(EditAnywhere, Category ="Door")
	float DoorClosingSpeed = 0.01;

};
