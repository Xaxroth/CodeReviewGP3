// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnemyStats.h"
#include "GameEndDataTracker.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "MeleeAttack.h"
#include "PlayerStats.h"
#include "Team12Character.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config=Game)
class ATeam12Character : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Dash Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* DashAction;

	/** Shoot Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* ShootAction;
	
	/** Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AttackAction;

	/** Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MutationAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UMeleeAttack* MeleeAttack;

	UPROPERTY(EditAnywhere , Category ="Components")
	TSubclassOf<class AActor> Follower;

	UPROPERTY(EditAnywhere , Category="Sensitivity")
	float Sensitivity = 1.5;
public:
	ATeam12Character();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPlayerStats* PlayerAttributes;

protected:
	virtual void BeginPlay();

public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;


public:

	UFUNCTION(BlueprintCallable)
   
	void ChangeSensitivity(float NewSensitivity);

	UPROPERTY(VisibleAnywhere, Category = "Dash")
	int CurrentDashCharges;
	
	UPROPERTY(VisibleAnywhere , BlueprintReadWrite)
	class USphereComponent* DashSphere;

	
	bool isDashing;
	bool WasAttacking;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent*  OverLappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp , int32 OtherBody ,bool bFromSweep,const FHitResult& SweepResult);
	
	void GroundCheck();
	
	UFUNCTION(BlueprintImplementableEvent)
	void ChangedHP();
	
	UPROPERTY(VisibleAnywhere ,Category = "Movement")
	bool isGrounded;

	UFUNCTION(BlueprintCallable)
	bool CanSwing();
	
	void UpdatePlayerStats();

	void StartAttack();

	void EndAttack();

	UFUNCTION(BlueprintCallable)
	int ReturnCanDash();

	UFUNCTION(BlueprintCallable)
	float  ReturnCurrentDashCounter();
	
	UFUNCTION(BlueprintCallable)
	void DashLineTrace();
	
	UFUNCTION(BlueprintCallable)
	void SetNewValuesMovement();

	UFUNCTION(BlueprintCallable)
	void SetNewValuesBody();

	UFUNCTION(BlueprintCallable)
	void UpdatePlayerSpeed();
	
	UFUNCTION(BlueprintCallable)
	void ChangeCanDash(int ChangeByThis);
	
	/*//Gun stuff
	
	UFUNCTION()
	void ReadyGun();
	
	UFUNCTION()
	void ShootGun();

	UPROPERTY(VisibleAnywhere, Category="Gun")
	int ShootsLeft;

	UPROPERTY(VisibleAnywhere, Category="Gun")
	int MaxShots ;
	
	UPROPERTY(VisibleAnywhere, Category="Gun")
	float ShootingCoolDown ;
	
	UPROPERTY(VisibleAnywhere, Category="Gun")
	float GunDamage;
	*/

	float AirTime;
	UFUNCTION(BlueprintImplementableEvent)
	void PlayGunAnimation();


	/* Events */
	
	UFUNCTION(BlueprintImplementableEvent)
	void AttackedInEncounter();
	UFUNCTION(BlueprintImplementableEvent)
	void Dashed();
	
	UFUNCTION(BlueprintImplementableEvent)
	void HitRock();
	UFUNCTION(BlueprintImplementableEvent)
	void HitSand();
	
	UFUNCTION(BlueprintImplementableEvent)
	void DoSwishSFX();
	
	UFUNCTION(BlueprintImplementableEvent)
	void DashIsReset();
	
protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	virtual void Tick(float deltaTime) override;

	void InitializeBaseValues();
	FTimerHandle DashResetTimer;


	/**Walking*/
	UFUNCTION(BlueprintCallable)
	void IsWalkingAndGrounded();

	UFUNCTION(BlueprintImplementableEvent)
	void StartedWalking();
	UFUNCTION(BlueprintImplementableEvent)
	void StopppedWalking();


	AActor* Ground;
	UPROPERTY(VisibleAnywhere, Category ="Ground")
	bool IsOnRock;
	
	UPROPERTY(EditAnywhere, Category ="Decal")
	TSubclassOf<AActor> SlashDecal;
	
	UFUNCTION(BlueprintCallable)
	bool GroundIsRock();
	
	bool isWalking;
	bool startedWalkingSFX;

	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }


private:
	//////////Melee////////////
	void CheckHitLocation();


	//////////BODY////////////
	UPROPERTY(VisibleAnywhere,  Category = "Player Body")
	float CameraHeight ;

	UPROPERTY(VisibleAnywhere, Category = "Player Body")
	float CameraForwardOffset ;

	UPROPERTY(VisibleAnywhere,  Category = "Player Body")
	float FieldOfView ;

	UPROPERTY(VisibleAnywhere, Category = "Player Body")
	float PlayerCapsuleHeight ;

	UPROPERTY(VisibleAnywhere, Category = "Player Body")
	float PlayerCapsuleWidth ;

	// GROUND MOVEMENT // 

	UPROPERTY(VisibleAnywhere,  Category = "Ground Movement")
	float MovementSpeed ;

	UPROPERTY(VisibleAnywhere,  Category = "Ground Movement")
	float GroundAcceleration ;

	UPROPERTY(VisibleAnywhere,  Category = "Ground Movement")
	float CurrentSpeed ;

	// AIRBORNE MOVEMENT //

	UPROPERTY(VisibleAnywhere, Category = "Airborne Movement")
	float MaxAirControl ;

	UPROPERTY(VisibleAnywhere,  Category = "Airborne Movement")
	float GravityModifier ;

	// Jump //

	UPROPERTY(VisibleAnywhere,  Category = "Jump")
	float JumpForce ;
	UPROPERTY(EditAnywhere, Category = "Jump")
	bool CanDoubleJump;
	UPROPERTY(EditAnywhere, Category = "Jump")
	bool DidDoubleJump;
	UPROPERTY(EditAnywhere, Category = "Jump")
	float DoubleJumpForce =2500;
	void DoJump();
	
	/** Dash Variables */
	UPROPERTY(EditAnywhere, Category = "Dash")
	int DashCharges = 1;
	UPROPERTY(VisibleAnywhere, Category = "Dash")
	float DashDamage;
	UPROPERTY(VisibleAnywhere, Category = "Dash")
	float DashDistance;
	UPROPERTY(EditAnywhere, Category = "Dash")
	float DashMomentum = 1500;
	UPROPERTY(VisibleAnywhere, Category = "Dash")
	float DashCoolDown = 6;

	UPROPERTY(EditAnywhere ,Category = "Dash")
	float DashSpeed = 0.1f;
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	bool isMovingForward;
	bool DashWasReset;
	UPROPERTY(EditAnywhere, Category = "Movement")
	FVector2D MovementVector;
	
	UPROPERTY(EditAnywhere, Category ="BunnyHopping")
	int BunnyHoppingForce = 1000;
	
	FVector DashPosition;
	FVector SavedDirection;

	float DashDurationCounter;
	float DashDurationTempCounter;
	int DashDurationFrameCounter;
	
	/** Dash Damaging shenanigans  */
	UPROPERTY(VisibleAnywhere, Category="Dash")
	TArray<UEnemyStats*> EnemiesDashedThrough;
	
	UPROPERTY(EditAnywhere, Category ="Component")
	UGameEndDataTracker* DataTracker;

	
	/** BunnyHop */
	void StopBunnyHop();
	void BunnyHop();
	bool canBunnyHop;
	void StopMoving();
	/** Dash Functions */
	void Dash();
	void DashPierceEnemy(FVector NewLineTraceEnd,FVector EndOfLine ,  AActor* Ignore);
	void ResetDashCoolDown();
	void EndOfDashMomentum();
	
};

