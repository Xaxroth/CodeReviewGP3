// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerStats.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "MeleeAttack.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM12_API UMeleeAttack : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	UMeleeAttack();

	UPROPERTY(EditAnywhere)
	UBoxComponent* WeaponHitbox;

	FTimerHandle TimerHandle_DisableHitbox;
	FTimerHandle TimerHandle_Cooldown;
	FTimerHandle TimerHandle_DamageDelay;

	// Properties //
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OffsetToPlayer = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HitboxLingerDuration = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HitboxLength = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HitboxWidth = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Cooldown = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Delay = 0.01f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Attributes")
	float MeleeDamage = 12.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Attributes")
	float BaseDamage = 12.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Attributes")
	float MeleeDRange;
	
	float CurrentMeleeCharge = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Attributes")
	float MaximumChargeMultiplier = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Attributes")
	float MeleeChargeRate = 0.001f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Attributes")
	float StrongAttackThreshold = 1.1f;

	bool CanSwing = true;
	bool HitboxActive = false;
	bool IsAttackProcessed = false;

	// Components // 
	UPROPERTY(VisibleAnywhere, Category = "Componenents")
	UPlayerStats* PlayerAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
		TArray<AActor*> HitEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
		TArray<AActor*> HitEggs;
	
	UPROPERTY(VisibleAnywhere, Category = "Componenents")
	AActor* HitLocation = Cast<ACharacter>(this->GetOwner()) ;

public:

	void DisableHitbox();

	void WeaponSwing();

	void UpdateWeaponStats();

	void EnableHitbox();

	void ChargeWeapon();

	void ResetWeapon();

	void SetHitboxLocation();

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	
	UPROPERTY(EditAnywhere, Category = "blood")
	TSubclassOf<AActor> Blood;

	void DestroyVFX( AActor* BloodInstance);
};
