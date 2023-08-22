// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "SpitProjectile.generated.h"

UCLASS()
class TEAM12_API ASpitProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpitProjectile();

	void EnableCollision();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileProperties")
	float LifeTime = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileProperties")
	float ProjectileDamage = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileProperties")
	float VerticalSpeed = 0.15f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileProperties")
	float ProjectileForce = 150000.f;

	bool HasDealtDamage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);

};
