// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStats.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM12_API UPlayerStats : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerStats();

	UPROPERTY()
	UPlayerStats* PlayerStatsComponent;

	UPROPERTY(EditAnywhere, Category = "Dash")
	float DashCoolDown = 3;
	UPROPERTY(EditAnywhere, Category = "Dash")
	float DashDamage = 110;
	UPROPERTY(EditAnywhere, Category = "Dash")
	float DashDistance = 3000;

	UFUNCTION(BlueprintCallable , Category= "Dash")
	bool DashState(bool IsAvailable);

	UFUNCTION(BlueprintCallable , Category= "Dash")
	float ReturnDashDamage();

	UFUNCTION(BlueprintCallable , Category= "Dash")
	float ReturnDashDistance();

	UFUNCTION(BlueprintCallable, Category = "Hookshot")
	float ReturnHookshotCooldown(float ChangeByThis);

	UPROPERTY(EditAnywhere, Category = "Player Resources")
	float MaxHealth = 100.f;
	
	
	UPROPERTY(VisibleAnywhere, Category = "Player Resources")
	float Health = 100.f;
	

	UPROPERTY(EditAnywhere, Category = "Player Resources")
	float MaxStamina;
	
    UPROPERTY(VisibleAnywhere, Category = "Player Resources")
    float Stamina;

	UFUNCTION(BlueprintCallable , Category= "Player Resources")
		void ChangeHealth(float ChangeByThis);

	UFUNCTION(BlueprintCallable , Category= "Player Resources")
		float ReturnHealth();

	UFUNCTION(BlueprintCallable , Category= "Player Resources")
		void ChangeStamina(float ChangeByThis);

	UFUNCTION(BlueprintCallable , Category= "Player Resources")
		float ReturnStamina();


	void UpdateDashCharges();




		//////////////////// FOR PLAYER CONTROLLER AND MOVEMENT AND STUFF //////////////////////

	// PLAYER BODY //

	// TODO: Move things that should be affected by mutation (max speed, FOV, camera height?) and make rest private/blueprint only

	// DO the thing from above after negotiating what can be changed and what should always be what it is
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Body")
	float CameraHeight = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Body")
	float CameraForwardOffset = -10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Body")
	float FieldOfView = 90.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Body")
	float PlayerCapsuleHeight = 96.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Body")
	float PlayerCapsuleWidth = 55.f;

	// GROUND MOVEMENT // 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground Movement")
	float MovementSpeed = 2500.f;

	float TemporarySpeedIncrease = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground Movement")
	float TimeUntilBuffWanes = 0.f;
	bool HasSpeedBuff;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground Movement")
	float GroundAcceleration = 15000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground Movement")
	float CurrentSpeed = 1.f;

	// AIRBORNE MOVEMENT //

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Airborne Movement")
	float MaxAirControl = 0.6f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Airborne Movement")
	float GravityModifier = 3.5f;

	//Jump//
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	float JumpForce = 1900;

	// MUTATIONS //

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mutations")
	float MutationLevel = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mutations")
	float MaximumMutationLevel = 100.f;

	// LEGENDARY Mutations //

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Legendary Mutations")
		bool DoubleJump = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Legendary Mutations")
		bool DashReset = false;

	
	// MELEE ATTACK PROPERTIES//	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee Attack")
	float MeleeDamage = 25.f;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee Attack")
	float MeleeRange = 25.f;

	// HOOKSHOT ATTACK PROPERTIES//

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee Attack")
		float HookshotRange = 4000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee Attack")
		float HookshotCooldown = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee Attack")
		int HookshotCharges = 1;

	// DASH ATTACK PROPERTIES//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
		int DashCharges = 1;


	//Gun Properties
	UPROPERTY(EditAnywhere, Category="Gun")
	int MaxShots =1;
	
	UPROPERTY(EditAnywhere, Category="Gun")
	float ShootingCoolDown = 10;
	
	UPROPERTY(EditAnywhere, Category="Gun")
	float GunDamage= 100;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable)
	float ReturnDashCoolDown();

	
};
