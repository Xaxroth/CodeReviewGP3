// Copyright Epic Games, Inc. All Rights Reserved.

#include "Team12Character.h"

#include "Egg.h"
#include "EnemyStats.h"
#include "Mutation.h"
#include "MutationActor.h"
#include "MutationCluster.h"
#include "../Team12GameMode.h"
#include "Team12Projectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerFollower.h"
#include "SpitProjectile.h"
#include "VectorTypes.h"
#include "Components/SphereComponent.h"
#include "Field/FieldSystemNoiseAlgo.h"


//////////////////////////////////////////////////////////////////////////
// ATeam12Character

ATeam12Character::ATeam12Character()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled;
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Default mesh component, replace with custom player arms / viewmodel
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));


	DashSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Dash Hit Sphere"));
	DashSphere -> SetSphereRadius(32);
	DashSphere -> SetCollisionProfileName("Trigger");


	DashSphere -> OnComponentBeginOverlap.AddDynamic(this , &ATeam12Character::OnOverlapBegin);
	
}

void ATeam12Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	PlayerAttributes = Cast<UPlayerStats>(this->GetComponentByClass(UPlayerStats::StaticClass()));
	DataTracker = Cast<UGameEndDataTracker>(this->GetComponentByClass(UGameEndDataTracker::StaticClass()));

	FActorSpawnParameters SpawnInfo;
	if(Follower)
	{
		AActor* temp=	GetWorld()-> SpawnActor<AActor>(Follower , this->GetActorLocation() ,this->GetActorRotation(),SpawnInfo);
		APlayerFollower* followerInstance = Cast<APlayerFollower>(temp);

		if(followerInstance)
		{
			followerInstance->player = this;
		}
	}
	
	SetNewValuesBody();
	SetNewValuesMovement();
	UpdatePlayerStats();
	InitializeBaseValues();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	MeleeAttack = Cast<UMeleeAttack>(GetComponentByClass(UMeleeAttack::StaticClass()));
	//ShootsLeft = MaxShots;
	CurrentDashCharges = DashCharges;
}

void ATeam12Character::InitializeBaseValues() 
{
	FirstPersonCameraComponent->SetRelativeLocation(FVector(CameraForwardOffset, 0.f, CameraHeight)); // Position the camera
	FirstPersonCameraComponent->SetFieldOfView(FieldOfView);
	GetCapsuleComponent()->InitCapsuleSize(PlayerCapsuleWidth, PlayerCapsuleHeight);
}

void ATeam12Character::UpdatePlayerStats() 
{
	UCharacterMovementComponent* PlayerMovementComponent = GetCharacterMovement();

	PlayerMovementComponent->MaxWalkSpeed = MovementSpeed;
	PlayerMovementComponent->MaxAcceleration = GroundAcceleration;
	PlayerMovementComponent->GravityScale = GravityModifier;
	PlayerMovementComponent->AirControl = MaxAirControl;
	FirstPersonCameraComponent->FieldOfView = FieldOfView;
}

//////////////////////////////////////////////////////////////////////////// Input

void ATeam12Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ATeam12Character::DoJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ATeam12Character::BunnyHop);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ATeam12Character::StartAttack);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &ATeam12Character::EndAttack);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATeam12Character::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ATeam12Character::StopMoving);
		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATeam12Character::Look);

		//Dash
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &ATeam12Character::DashLineTrace);

        //Gun
      //  EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &ATeam12Character::ReadyGun);
        		
	}
}

void ATeam12Character::StartAttack() 
{
	if (MeleeAttack != nullptr && MeleeAttack->CanSwing) 
	{
		MeleeAttack->ChargeWeapon();
	}
}


void ATeam12Character::EndAttack()
{
	if (MeleeAttack != nullptr)
	{
		MeleeAttack->WeaponSwing();
		CheckHitLocation();
	}

	AttackedInEncounter();
}


void ATeam12Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	 MovementVector = Value.Get<FVector2D>();
	SavedDirection.Y = MovementVector.Y;
	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
	if(MovementVector.X == 0)
	{
	isMovingForward= true;
	}else
	{
		isMovingForward= false;
	}


	if(MovementVector != FVector2d(0,0))
	{
		isWalking = true;
	}else
	{
		isWalking= false;
	}

}

void ATeam12Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>() * Sensitivity;

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ATeam12Character::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	DashSphere->SetWorldLocation(GetActorLocation());
	GroundCheck();
	IsWalkingAndGrounded();
	
	if(DashPosition != FVector(0,0,0))
	{
		Dash();
	}
}

void ATeam12Character::GroundCheck()
{

	FVector groundCheckLocation = GetActorLocation() - FVector(0.0f, 0.0f, PlayerCapsuleHeight/2);

	FHitResult hitResult;
	
	FCollisionQueryParams TraceParams(FName(TEXT("")), true, GetOwner());


	//Add More Actors to ignore
	TraceParams.AddIgnoredActor(this);
	
	// Raycast out to this distance
	isGrounded = GetWorld()->LineTraceSingleByObjectType(
		OUT hitResult,
		groundCheckLocation,
		groundCheckLocation - FVector(0,0,250),
		FCollisionObjectQueryParams(),
		TraceParams
	);

	AActor* temp = hitResult.GetActor();
	if(temp && temp->ActorHasTag("Sand"))
	{
		IsOnRock =false;
	}
	else if(temp && !temp->ActorHasTag("Sand"))
	{
		IsOnRock =true;
	}
	
	if (isGrounded) 
	{
		DidDoubleJump = false;
		canBunnyHop= true;
		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(
			UnusedHandle, this, &ATeam12Character::StopBunnyHop, 0.5f, false);

	}
	else
	{
		canBunnyHop =false;
	}
	
}



 void ATeam12Character::DashLineTrace()
{
	if(CurrentDashCharges>0)
	{
		
		isDashing=true;
		DataTracker->GameMode->dashes++;
		Dashed();
		
		// Set up parameters for getting the player view
		FVector  PlayerViewPointLocation;
		FRotator PlayerViewPointRotation;

	
		// Get player viewpoint and set these parameters
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation , OUT PlayerViewPointRotation);

		// Extend the 1 from that vector to whatever dash range is 
		FVector LineTraceEnd = FVector(PlayerViewPointLocation.X ,PlayerViewPointLocation.Y,PlayerViewPointLocation.Z) + PlayerViewPointRotation.Vector() * DashDistance;
	
		// Set parameters to use line tracing
		FHitResult Hit; 
		FCollisionQueryParams TraceParams(FName(TEXT("")), true, GetOwner());


		//Add More Actors to ignore
		TraceParams.AddIgnoredActor(this);

		// Raycast out to this distance
		GetWorld()->LineTraceSingleByObjectType(
			OUT Hit,
			PlayerViewPointLocation,
			LineTraceEnd,
			FCollisionObjectQueryParams(),
			TraceParams
		);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, Hit.Location.ToString()  );

	
		AActor* ActorHit = Hit.GetActor();

		if (ActorHit) {
			DrawDebugLine(GetWorld() ,FVector(PlayerViewPointLocation.X ,PlayerViewPointLocation.Y,PlayerViewPointLocation.Z) ,LineTraceEnd,FColor::Red , false, 100, 0,3 );
			DashPosition = Hit.Location;

			UEnemyStats* EnemyHit = Cast<UEnemyStats>(ActorHit->GetComponentByClass(UEnemyStats::StaticClass()));
			AEgg* egg = Cast<AEgg>(ActorHit);
			ASpitProjectile* projectile = Cast<ASpitProjectile>(ActorHit);
			if(EnemyHit!= NULL)
			{
				//EnemyHit->ChangeHealth(-DashDamage);
				DashPierceEnemy(Hit.Location,LineTraceEnd , ActorHit);
			}else if (egg!= NULL)
			{
				DashPierceEnemy(Hit.Location,LineTraceEnd , ActorHit);
			}else if (projectile!= NULL)
			{
				DashPierceEnemy(Hit.Location,LineTraceEnd , ActorHit);

			}
		
		}else
		{
			DashPosition = LineTraceEnd;
			DrawDebugLine(GetWorld() ,FVector(PlayerViewPointLocation.X ,PlayerViewPointLocation.Y,PlayerViewPointLocation.Z) ,LineTraceEnd,FColor::Red , false, 100, 0,3 );

		}
		CurrentDashCharges--;



		//Reset Dashes//

			GetWorld()->GetTimerManager().ClearTimer(DashResetTimer);
			if(!GetWorld()->GetTimerManager().TimerExists(DashResetTimer)){
			GetWorldTimerManager().SetTimer(
				DashResetTimer, this, &ATeam12Character::ResetDashCoolDown, DashCoolDown, false);
			}
		
	}
}
void ATeam12Character::Dash()
{
	const FVector ForwardDir = FirstPersonCameraComponent->GetComponentRotation().Vector();
	FHitResult* HitThis = NULL;

	FVector UpdatedPosition = FMath::Lerp(this->GetActorLocation() ,DashPosition ,DashSpeed);


	// this makes sure that the Lerping ends when nearing the end of the line
	DashDurationTempCounter++;
	DashDurationTempCounter = DashSpeed/ DashDurationTempCounter;
	DashDurationCounter += DashDurationTempCounter;


if(DashDurationCounter>2)
{
	//when dash ends reset everything to be used for the next dash and apply momentum
	EndOfDashMomentum();
	DashPosition = FVector(0,0,0);
	DashDurationTempCounter = 0;
	DashDurationCounter =0;
	isDashing=false;

	for(int i = 0; i < EnemiesDashedThrough.Num(); i++) {
		if(EnemiesDashedThrough[i] != NULL)
		{
			EnemiesDashedThrough[i]->WasDashedThrough=false;
			EnemiesDashedThrough[i]->GetOwner()->SetActorEnableCollision(true);
		}		

	}
	
}
	
	 this->SetActorRelativeLocation(UpdatedPosition , true,HitThis,ETeleportType::TeleportPhysics);

}

void ATeam12Character::DashPierceEnemy(FVector NewLineTraceEnd , FVector EndOfLine, AActor* Ignore)
{
	
	// Extend the 1 from that vector to whatever dash range is 
	FVector LineTraceEnd = EndOfLine ;
	
	// Set parameters to use line tracing
	FHitResult Hit; 
	FCollisionQueryParams TraceParams(FName(TEXT("")), true, GetOwner());


	//Add More Actors to ignore
	TraceParams.AddIgnoredActor(Ignore);
	TraceParams.AddIgnoredActor(this);
	TraceParams.AddIgnoredActor(GetOwner());
	// Raycast out to this distance
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		NewLineTraceEnd,
		LineTraceEnd,
		FCollisionObjectQueryParams(),
		TraceParams
	);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, Hit.Location.ToString()  );

	
	AActor* ActorHit = Hit.GetActor();

	if (ActorHit) {
		DrawDebugLine(GetWorld() , NewLineTraceEnd ,LineTraceEnd,FColor::Blue , false, 100, 0,3 );
		DashPosition = Hit.Location;

		//check if the new target hit is an enemy
		UEnemyStats* EnemyHit = Cast<UEnemyStats>(ActorHit->GetComponentByClass(UEnemyStats::StaticClass()));

		//do another pierce dash if there is another enemy
		if(EnemyHit != NULL)
		{
			//EnemyHit->ChangeHealth(-DashDamage);
			DashPierceEnemy(Hit.Location,LineTraceEnd , ActorHit);
			UE_LOG(LogTemp, Warning, TEXT("The Actor's name is %s"), *ActorHit->GetName());
		}
		//Draw a debug line and set the target position to the end of the dash distance
	}else
	{
		DashPosition = LineTraceEnd;
		DrawDebugLine(GetWorld() ,DashPosition ,LineTraceEnd,FColor::Blue , false, 100, 0,3 );

	}

}

//apply momentum at the end of a dash 
void ATeam12Character::EndOfDashMomentum()
{
	LaunchCharacter( this->GetActorForwardVector() * DashMomentum, true, true);
}
//Allow Dash after A certain amount of time
void ATeam12Character::ResetDashCoolDown()
{
	CurrentDashCharges++;
	DashIsReset();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Added A charge"));
	if(CurrentDashCharges>DashCharges)
	{
		CurrentDashCharges = DashCharges;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Stop dash resetting"));
	}else if (CurrentDashCharges<DashCharges)
	{
		GetWorld()->GetTimerManager().ClearTimer(DashResetTimer);
		if(!GetWorld()->GetTimerManager().TimerExists(DashResetTimer)){
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("give me more"));
		GetWorldTimerManager().SetTimer(
		DashResetTimer, this, &ATeam12Character::ResetDashCoolDown, DashCoolDown, false);
		}
	}
	
}
//Return values for designers to use//
int ATeam12Character::ReturnCanDash()
{
	return CurrentDashCharges;
}
float ATeam12Character::ReturnCurrentDashCounter()
{
	return DashDurationCounter;
}
bool ATeam12Character::CanSwing() 
{
	bool bCanSwing = MeleeAttack->CanSwing;

	return MeleeAttack->CanSwing;
}

void ATeam12Character::BunnyHop()
{
	if(SavedDirection.Y  == 1&& isMovingForward && isGrounded )
	{
		LaunchCharacter( (FVector(0,0,1) + FirstPersonCameraComponent->GetForwardVector()) * BunnyHoppingForce , true, true);
	canBunnyHop=false;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("BunnyHopping"));
	}else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Not BunnyHopping"));

	}
}

void ATeam12Character::StopBunnyHop()
{
	canBunnyHop=false;
}


void ATeam12Character::SetNewValuesMovement()
{
	// Basic Movement//
	MovementSpeed =PlayerAttributes->MovementSpeed;
	GroundAcceleration = PlayerAttributes->GroundAcceleration;
	CurrentSpeed = PlayerAttributes->CurrentSpeed;
	MaxAirControl = PlayerAttributes->MaxAirControl;
	GravityModifier = PlayerAttributes->GravityModifier;

	// Dash 
	DashDamage = PlayerAttributes-> DashDamage;
	DashDistance = PlayerAttributes -> DashDistance;
	DashCoolDown = PlayerAttributes -> DashCoolDown;
	DashCharges = PlayerAttributes -> DashCharges;

	GetWorld()->GetTimerManager().ClearTimer(DashResetTimer);
	if(!GetWorld()->GetTimerManager().TimerExists(DashResetTimer))
	{
		GetWorldTimerManager().SetTimer(
		DashResetTimer, this, &ATeam12Character::ResetDashCoolDown, DashCoolDown, false);
	}

	JumpForce = PlayerAttributes -> JumpForce;
	CanDoubleJump = PlayerAttributes-> DoubleJump;
}

void ATeam12Character::SetNewValuesBody()
{
	CameraHeight = PlayerAttributes->CameraHeight;
	CameraForwardOffset = PlayerAttributes->CameraForwardOffset;
	FieldOfView = PlayerAttributes->FieldOfView;
	PlayerCapsuleHeight = PlayerAttributes->PlayerCapsuleHeight;
	PlayerCapsuleWidth = PlayerAttributes->PlayerCapsuleWidth;

	FirstPersonCameraComponent->SetRelativeLocation(FVector(CameraForwardOffset, 0.f, CameraHeight)); // Position the camera
	FirstPersonCameraComponent->SetFieldOfView(FieldOfView);
	GetCapsuleComponent()->InitCapsuleSize(PlayerCapsuleWidth, PlayerCapsuleHeight);
}
void ATeam12Character::UpdatePlayerSpeed() 
{
	MovementSpeed = PlayerAttributes->MovementSpeed;
	GroundAcceleration = PlayerAttributes->GroundAcceleration;
	CurrentSpeed = PlayerAttributes->CurrentSpeed;
}

void ATeam12Character::ChangeCanDash(int ChangeByThis)
{
CurrentDashCharges=  CurrentDashCharges +ChangeByThis;

	if(CurrentDashCharges > DashCharges)
	{
		CurrentDashCharges = DashCharges;
	}
	
	GetWorld()->GetTimerManager().ClearTimer(DashResetTimer);
}

void ATeam12Character::DoJump()
{
	bool wasARegularJump = false;
	if(isGrounded)
	{
		Jump();
		wasARegularJump=true;
	}else if (!DidDoubleJump && CanDoubleJump && !isGrounded && !wasARegularJump)
	{
		DidDoubleJump= true;

		if(isMovingForward){
		LaunchCharacter( FVector(this->GetActorForwardVector().X* (DoubleJumpForce/8),this->GetActorForwardVector().Y * (DoubleJumpForce/8), 1 * DoubleJumpForce)  , true, true);
			
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Trying to double jump and forward"));
		}else
		{
			LaunchCharacter( FVector(0,0, 1 * DoubleJumpForce)  , true, true);
			
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Trying to double jump"));
		}
		
	}
}

void ATeam12Character::StopMoving()
{
	isMovingForward=false;
	isWalking=false;
}


void ATeam12Character::OnOverlapBegin(UPrimitiveComponent*  OverLappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp , int32 OtherBody ,bool bFromSweep,const FHitResult& SweepResult)
{


	 UEnemyStats* enemy = Cast<UEnemyStats>(OtherActor->GetComponentByClass(UEnemyStats::StaticClass()));


		AEgg* egg = Cast<AEgg>(OtherActor);

	ASpitProjectile* spit = Cast<ASpitProjectile>(OtherActor);

	if (enemy && isDashing && !enemy->WasDashedThrough)
	{
		enemy->ChangeHealth(-DashDamage);
		enemy->WasDashedThrough= true;
		EnemiesDashedThrough.Add(enemy);
		enemy->GetOwner()->SetActorEnableCollision(false);
	}
	
	if (egg && isDashing )
	{
		egg->ChangeHealth(-DashDamage);
		egg->SetActorEnableCollision(false);
	}
	if (spit && isDashing )
    	{
    		spit->SetActorEnableCollision(false);
    	}
	
}

void ATeam12Character::CheckHitLocation()
{

	FVector  PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	
	// Get player viewpoint and set these parameters
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation , OUT PlayerViewPointRotation);

	// Extend the 1 from that vector to whatever dash range is 
	FVector LineTraceEnd = FVector(PlayerViewPointLocation.X ,PlayerViewPointLocation.Y,PlayerViewPointLocation.Z) + PlayerViewPointRotation.Vector() * 400;
	
	// Set parameters to use line tracing
	FHitResult Hit; 
	FCollisionQueryParams TraceParams(FName(TEXT("")), true, GetOwner());


	//Add More Actors to ignore
	TraceParams.AddIgnoredActor(this);

	// Raycast out to this distance
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(),
		TraceParams
	);

	DrawDebugLine(GetWorld() ,FVector(PlayerViewPointLocation.X ,PlayerViewPointLocation.Y,PlayerViewPointLocation.Z) ,LineTraceEnd,FColor::Blue , false, 100, 0,3 );
	FVector HitLocation = Hit.Location;

	if(HitLocation == FVector(0,0,0))
	{
		DoSwishSFX();
	}else if (!Hit.GetActor()->ActorHasTag("Sand") && Hit.GetActor()->GetComponentByClass(UEnemyStats::StaticClass())==NULL )
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Hit Something that is a rock"));
		FActorSpawnParameters SpawnInfo;

		if(SlashDecal){
		AActor* temp=	GetWorld()-> SpawnActor<AActor>(SlashDecal ,HitLocation ,this->GetActorRotation(),SpawnInfo);

		temp->SetActorRotation(Hit.Normal.ToOrientationQuat());
		}
		HitRock();
	}else if(Hit.GetActor() && Hit.GetActor()->ActorHasTag("Sand"))
	{
		HitSand();
	}
	
}




void ATeam12Character::IsWalkingAndGrounded()
{

	if(isGrounded && isWalking)
	{

		if(!startedWalkingSFX){
		StartedWalking();
			startedWalkingSFX=true;
		}
	}else 
	{
		StopppedWalking();
		startedWalkingSFX=false;
	}
	
}

bool ATeam12Character::GroundIsRock()
{
	return IsOnRock;
}

void ATeam12Character::ChangeSensitivity(float NewSensitivity)
{
	Sensitivity= NewSensitivity;
}
