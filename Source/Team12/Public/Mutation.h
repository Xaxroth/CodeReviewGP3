// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStats.h"
#include "Mutation.generated.h"

UENUM(BlueprintType)
enum class MutationType : uint8
{
	None UMETA(DisplayName = "None"),
	MaxDamage UMETA(DisplayName = "Damage"),
	MaxHealth UMETA(DisplayName = "Health"),
	MaxSpeed UMETA(DisplayName = "Speed"),
	DashDamage UMETA(DisplayName = "DashDamage"),
	DashCooldown UMETA(DisplayName = "DashCooldown"),
	DashDistance UMETA(DisplayName = "DashDistance"),
	DashCharges UMETA(DisplayName = "DashCharges"),
	FieldOfView UMETA(DisplayName = "FieldOfView"),
	HookshotRange UMETA(DisplayName = "HookshotRange"),
	HookshotCooldown UMETA(DisplayName = "HookshotCooldown"),
	HookshotCharges UMETA(DisplayName = "HookshotCooldown"),
	MeleeRange UMETA(DisplayName = "AttackRange"),
	JumpHeight UMETA(DisplayName = "JumpHeight"),
	MaxShots UMETA(DisplayName = "MaxShots"),
	ShootingCoolDown UMETA(DisplayName = "ShootingCoolDown"),
	GunDamage UMETA(DisplayName = "GunDamage"),
	CanDoubleJump UMETA(DisplayName = "CanDoubleJump")
};

UENUM(BlueprintType)
enum class MutationRarity : uint8
{
	None UMETA(DisplayName = "None"),
	Common UMETA(DisplayName = "Common"),
	Rare UMETA(DisplayName = "Rare"),
	Epic UMETA(DisplayName = "Epic"),
	Legendary UMETA(DisplayName = "Legendary")
};

UENUM(BlueprintType)
enum class MutationEffect : uint8
{
	None UMETA(DisplayName = "None"),
	Add UMETA(DisplayName = "Add"),
	Subtract UMETA(DisplayName = "Subtract"),
	Multiply UMETA(DisplayName = "Multiply"),
	Divide UMETA(DisplayName = "Divide")
};

UENUM(BlueprintType)
enum class LegendaryMutations : uint8
{
	None UMETA(DisplayName = "None"),
	DoubleJump UMETA(DisplayName = "DoubleJump"),
	DashReset UMETA(DisplayName = "DashReset")
};

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM12_API UMutation : public UActorComponent
{
	GENERATED_BODY()

public:	

	UMutation();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UPlayerStats* PlayerAttributes;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mutation Name")
		FString MutationName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mutation Data")
		MutationRarity Rarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mutation Data")
		MutationEffect Effect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mutation Data")
		MutationType Type;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "Rarity == MutationRarity::Legendary"), Category = "Mutation Data")
		LegendaryMutations LegendaryEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mutation Data")
		float ChangeFloatValue = 0;

		float VariableToChange;


	// MUTATION METER // 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mutation Meter")
		bool IncreasesMutationLevel = false;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bIncreasesMutationLevel"), Category = "Mutation Meter")
		float MutationAmount = 0;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
