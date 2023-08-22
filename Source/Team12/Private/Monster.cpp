// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "SpitProjectile.h"
#include "../Team12Character.h"

// Sets default values
AMonster::AMonster()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    FieldOfViewArea = CreateDefaultSubobject<UCapsuleComponent>(TEXT("FieldOfView"));
    RootComponent = FieldOfViewArea;

    FieldOfViewArea->InitCapsuleSize(AggroRange, AggroRange);
    FieldOfViewArea->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

    FieldOfViewArea->OnComponentBeginOverlap.AddDynamic(this, &AMonster::OnBeginOverlap);
    FieldOfViewArea->OnComponentEndOverlap.AddDynamic(this, &AMonster::OnEndOverlap);
}

void AMonster::BeginPlay()
{
    Super::BeginPlay();

    TargetLocation = GetRandomTargetLocation();
    PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);    
    PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    PlayerPawn = PlayerController->GetPawn();

    USkeletalMeshComponent* SkeletalMeshComponent = FindComponentByClass<USkeletalMeshComponent>();

}

void AMonster::SetMonsterState(int32 NewState) 
{
    MonsterAnimationNumber = NewState;
}

void AMonster::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    DistanceToPlayer = GetDistanceTo(PlayerCharacter);

    if (State == MonsterState::Dead && !Dormant) 
    {
        GetWorldTimerManager().ClearTimer(SpitAttackTimerHandle);
        GetWorldTimerManager().ClearTimer(StopSpitTimerHandle);
        GetWorldTimerManager().ClearTimer(ReturnTimerHandle);
        GetWorldTimerManager().ClearTimer(AttackStateTimerHandle);
        SetMonsterState(2);
        Dormant = true;
    }

    if (DistanceToPlayer >= 7000 && State != MonsterState::Dead) 
    {
        Dormant = true;
    }

    if (DistanceToPlayer <= 7000 && State != MonsterState::Dead) 
    {
        Dormant = false;
    }

    if (Dormant) 
    {
        return;
    }
    Behavior(DeltaTime);
}

void AMonster::FacePlayer(float DeltaTime) 
{
    FVector Direction = PlayerPawn->GetActorLocation() - GetActorLocation();
    FRotator CurrentRotation = GetActorRotation();
    FRotator TargetRotation = Direction.Rotation();
    Direction.Z = 0.f;

    TargetRotation.Pitch = 0.f;
    TargetRotation.Roll = 0.f;

    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, EnemyTurnSpeed);

    SetActorRotation(NewRotation);
}

void AMonster::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->IsA(ATeam12Character::StaticClass()) && State == MonsterState::Searching)
    {
        GetWorldTimerManager().SetTimer(AttackStateTimerHandle, this, &AMonster::ChargeAttack, TimeUntilNextAttack, true);
        TargetLocation = OtherActor->GetActorLocation();
        OriginalLocation = GetActorLocation();
        State = MonsterState::Hunting;
    }
}

void AMonster::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

FVector AMonster::GetRandomTargetLocation()
{
    float Range = 1000.0f; // Adjust this value to define the movement range when monster is idle

    FVector CurrentLocation = GetActorLocation();

    float RandomX = FMath::FRandRange(CurrentLocation.X - Range, CurrentLocation.X + Range);
    float RandomY = FMath::FRandRange(CurrentLocation.Y - Range, CurrentLocation.Y + Range);
    float RandomZ = CurrentLocation.Z;

    FVector RandomLocation(RandomX, RandomY, RandomZ);
    return RandomLocation;
}

void AMonster::MoveToTargetLocation(float DeltaTime)
{
    // Calculate the movement direction
    FVector CurrentLocation = GetActorLocation();
    FVector Direction = TargetLocation - CurrentLocation;
    Direction.Normalize();

    // Perform a raycast to check for obstacles along the desired path
    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this); // Ignore the actor itself to avoid self-collision

        // Perform the raycast
        bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, CurrentLocation, TargetLocation, ECC_Visibility, Params);

        // Check if the raycast hit an obstacle
        if (bHit)
        {
            // Adjust the target location to a nearby unobstructed position
            TargetLocation = HitResult.ImpactPoint + HitResult.ImpactNormal * AvoidanceRange;
        }
        else
        {
            FRotator CurrentRotation = GetActorRotation();
            FRotator TargetRotation = Direction.Rotation();

            // Interpolate the rotation towards the target rotation
            FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, EnemyTurnSpeed);

            SetActorRotation(NewRotation);
        }


        // Calculate the distance to the target location
        float Distance = FVector::Dist(CurrentLocation, TargetLocation);

        // Check if the actor has reached the target location
        if (Distance <= MinDistance)
        {
            // Generate a new random target location
            TargetLocation = GetRandomTargetLocation();
        }
        else
        {
            // Move the actor towards the target location
            FVector NewLocation = CurrentLocation + (Direction * MovementSpeed * DeltaTime);
            SetActorLocation(NewLocation);
        }
}

void AMonster::ChargeAttack() 
{
    ShootingSpit = false;
    State = MonsterState::Attacking;
    GetWorldTimerManager().SetTimer(StopSpitTimerHandle, this, &AMonster::ContinueChase, 0.6f, false);
}

void AMonster::ContinueChase() 
{
    ShootingSpit = true;
    State = MonsterState::Shooting;
    GetWorldTimerManager().SetTimer(ReturnTimerHandle, this, &AMonster::Recharge, 0.5f, false);
}

void AMonster::Recharge() 
{
    ShootingSpit = false;
    State = MonsterState::Hunting;
}

void AMonster::Chase(float DeltaTime) 
{
    FacePlayer(DeltaTime);
}

void AMonster::ReturnBack(float DeltaTime) 
{
    FVector Direction = OriginalLocation - GetActorLocation(); // Calculate the direction from current location to original location
    Direction.Normalize();

    FVector NewLocation = GetActorLocation();
    FVector XYMovement = Direction * MovementSpeed * DeltaTime;

    FRotator CurrentRotation = GetActorRotation();
    FRotator TargetRotation = Direction.Rotation();

    Direction.Z = 0.f;
    XYMovement.Z = 0.0f;
    NewLocation += XYMovement;

    TargetRotation.Pitch = 0.f;
    TargetRotation.Roll = 0.f;

    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, EnemyTurnSpeed);

    SetActorRotation(NewRotation);
    SetActorLocation(NewLocation);

    // Calculate the distance to the original location
    float Distance = FVector::Dist(GetActorLocation(), OriginalLocation);

    // Check if the actor has reached the original location
    if (Distance <= 100.f)
    {
        State = MonsterState::Searching;
    }
}

void AMonster::SpitAttack() 
{
    if (ShootingSpit && State == MonsterState::Shooting)
    {
        FVector SpawnLocation = GetActorLocation() + FVector(0.0f, 0.0f, ShootOffset);

        // Get the player's location
        PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
        FVector PlayerLocation = PlayerCharacter->GetActorLocation();

        // Calculate the rotation to face the player's position
        FVector DirectionToPlayer = PlayerLocation - SpawnLocation;
        FRotator SpawnRotation = DirectionToPlayer.Rotation();

        ASpitProjectile* SpitProjectile = GetWorld()->SpawnActor<ASpitProjectile>(SpitProjectileBase, SpawnLocation, SpawnRotation);
    }
}

void AMonster::Behavior(float DeltaTime)
{

if(!IsDead){
    switch (State)
    {
    case MonsterState::Searching:
        MonsterAnimationNumber = 0;
        EnemyTurnSpeed = 1.f;
        AggroRange = 4000.f;
        FieldOfViewArea->SetCapsuleSize(AggroRange, AggroRange);
        MoveToTargetLocation(DeltaTime);
        PlayerInView = false;
        MonsterIsSearching();
        break;
    case MonsterState::Hunting:
        MonsterAnimationNumber = 0;
        EnemyTurnSpeed = 4.f;
        MovementSpeed = 0.f;
        AggroRange = 7000.f;
        FieldOfViewArea->SetCapsuleSize(AggroRange, AggroRange);
        Chase(DeltaTime);
        MonsterIsHunting();
        break;
    case MonsterState::Returning:
        MonsterAnimationNumber = 0;
        EnemyTurnSpeed = 2.f;
        ReturnBack(DeltaTime);
        MonsterIsReturning();
        break;
    case MonsterState::Attacking:
        MonsterAnimationNumber = 1;
        FacePlayer(DeltaTime);
        GetWorldTimerManager().SetTimer(SpitAttackTimerHandle, this, &AMonster::SpitAttack, PukingDuration, true);
        EnemyTurnSpeed = 8.f;
        break;
    case MonsterState::Dead:
        MonsterAnimationNumber = 2;
        IsDead=true;
    case MonsterState::Shooting:
        if (!Dormant) 
        {
            SpitAttack();
            FacePlayer(DeltaTime);
            EnemyTurnSpeed = 8.f;
            MonsterIsAttacking();
        }
    }
    }
}

