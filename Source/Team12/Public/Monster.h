// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Encounter.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Monster.generated.h"

UENUM(BlueprintType)
enum class MonsterState : uint8
{
	Searching UMETA(DisplayName = "Searching"),
	Hunting UMETA(DisplayName = "Hunting"),
	Returning UMETA(DisplayName = "Returning"),
	Attacking UMETA(DisplayName = "Attacking"),
	Shooting UMETA(DisplayName = "Shooting"),
	Dead UMETA(DisplayName = "Dead")
};

UCLASS()
class TEAM12_API AMonster : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMonster();

public:

	float FieldOfView = 90;

	float MovementSpeed = 0.0f; // Adjust this value to control the movement speed
	float MinDistance = 1000.0f; // Minimum distance to consider reaching the target location

	float AggroRange = 5000.f;

	bool PlayerInView = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
		class UAnimInstance* AnimationInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
		TEnumAsByte<MonsterState> AnimationState;

	UFUNCTION(BlueprintCallable, Category = "Animation")
		void SetMonsterState(int32 NewState);		

	UFUNCTION(BlueprintCallable, Category = "Animation")
	int GetMonsterAnimationNumber() { return MonsterAnimationNumber; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	int MonsterAnimationNumber; 

	bool Dormant = false;

    bool IsDead;
	bool ShootingSpit = false;

	float AvoidanceRange =  1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Behavior")
		float PukingDuration = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Behavior")
		float TimeUntilNextAttack = 2.5f;

	float EnemyTurnSpeed = 5.f;

	void CheckForPlayer();

	void MoveToTargetLocation(float DeltaTime);

	void ChargeAttack();

	void ContinueChase();

	void Recharge();

	void Chase(float DeltaTime);

	void ReturnBack(float DeltaTime);

	void SpitAttack();

	void Behavior(float DeltaTime);

	FVector GetRandomTargetLocation();
	//FVector CurrentLocation;
	FVector TargetLocation;

	FVector OriginalLocation;

	FTimerHandle MonsterProcessingTimer;
	float WaitTime = 2.0f;

	float ShootOffset = 200.f;

	float DistanceToPlayer;

	FTimerHandle SpitAttackTimerHandle;

	FTimerHandle AttackStateTimerHandle;

	FTimerHandle StopSpitTimerHandle;

	FTimerHandle ReturnTimerHandle;

	APawn* PlayerPawn;
	ACharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mutation Data")
		MonsterState State;

	APlayerController* PlayerController;

	UPROPERTY(EditDefaultsOnly, Category = "SpitAttack")
	TSubclassOf<AActor> SpitProjectileBase;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void TimerCallback();

	UPROPERTY(VisibleAnywhere)
		UCapsuleComponent* FieldOfViewArea;

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void FacePlayer(float DeltaTime);

	AEncounter* SavedEncounter;


	//Events//
	UFUNCTION(BlueprintImplementableEvent)
	void MonsterIsDead();


		
	UFUNCTION(BlueprintImplementableEvent)
	void MonsterIsSearching();
		
	UFUNCTION(BlueprintImplementableEvent)
	void MonsterIsHunting();
	
	UFUNCTION(BlueprintImplementableEvent)
	void MonsterIsReturning();

	UFUNCTION(BlueprintImplementableEvent)
	void MonsterIsAttacking();
	
};
