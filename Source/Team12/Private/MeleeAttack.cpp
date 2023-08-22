// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeAttack.h"
#include "Egg.h"
#include "EnemyStats.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Team12/Team12Character.h"


// Sets default values for this component's properties
UMeleeAttack::UMeleeAttack()
{
    PrimaryComponentTick.bCanEverTick = true;

   WeaponHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));
   WeaponHitbox->SetBoxExtent(FVector(HitboxLength, HitboxWidth, 100.f));
   WeaponHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
   WeaponHitbox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
   
   SetHitboxLocation();
}

void UMeleeAttack::BeginPlay()
{
    Super::BeginPlay();

    PlayerAttributes = Cast<UPlayerStats>(GetOwner()->GetComponentByClass(UPlayerStats::StaticClass()));
    WeaponHitbox = Cast<UBoxComponent>(GetOwner()->GetComponentByClass(UBoxComponent::StaticClass()));

    AActor* Owner = GetOwner();

    UPrimitiveComponent* Primitive = WeaponHitbox;

    if (Primitive)
    {
        Primitive->OnComponentBeginOverlap.AddDynamic(this, &UMeleeAttack::OnComponentBeginOverlap);
    }

    if (WeaponHitbox != NULL) 
    {
        WeaponHitbox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    }

    if(PlayerAttributes != NULL){
    
        MeleeDamage = PlayerAttributes->MeleeDamage;
    }
}

void UMeleeAttack::SetHitboxLocation() 
{
    ACharacter* PlayerCharacter = Cast<ACharacter>(GetOwner());
    if (PlayerCharacter != nullptr)
    {
        APlayerController* PlayerController = Cast<APlayerController>(PlayerCharacter->GetController());
        if (PlayerController != nullptr)
        {
            FVector CameraLocation;
            FRotator CameraRotation;
            PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

            FVector ForwardVector = CameraRotation.Vector();
            FVector BoxOffset = ForwardVector * OffsetToPlayer;
            FVector HitboxLocation = CameraLocation + BoxOffset;

            WeaponHitbox->SetBoxExtent(FVector(HitboxLength, HitboxWidth, 100.f));
            WeaponHitbox->SetWorldLocation(HitboxLocation);
            WeaponHitbox->SetWorldRotation(CameraRotation);
        }
    }
}

void UMeleeAttack::ChargeWeapon() 
{
    if (CanSwing) 
    {
        if (CurrentMeleeCharge < MaximumChargeMultiplier) 
        {
            CurrentMeleeCharge += MeleeChargeRate;
        }
        else 
        {
            CurrentMeleeCharge = MaximumChargeMultiplier;
        }
    }
}


void UMeleeAttack::WeaponSwing()
{
    UpdateWeaponStats();

    if (CanSwing) 
    {
        if (CurrentMeleeCharge > StrongAttackThreshold) 
        {
            MeleeDamage = BaseDamage * CurrentMeleeCharge;
        }
        else 
        {
            MeleeDamage = BaseDamage;
        }

        SetHitboxLocation();
        CurrentMeleeCharge = 1.0f;

        GetWorld()->GetTimerManager().SetTimer(TimerHandle_DamageDelay, this, &UMeleeAttack::EnableHitbox, Delay, false);
    }
}

void UMeleeAttack::UpdateWeaponStats() 
{
    if (PlayerAttributes != nullptr) 
    {
        BaseDamage = PlayerAttributes->MeleeDamage;
    }
}

void UMeleeAttack::EnableHitbox()
{
    CanSwing = false;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_DisableHitbox, this, &UMeleeAttack::DisableHitbox, HitboxLingerDuration, false);
    WeaponHitbox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    HitboxActive = true;
}

void UMeleeAttack::DisableHitbox()
{
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_Cooldown, this, &UMeleeAttack::ResetWeapon, Cooldown, false);
    WeaponHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    HitboxActive = false;
}

void UMeleeAttack::ResetWeapon() 
{
    MeleeDamage = BaseDamage;
    HitEggs.Empty();
    HitEnemies.Empty();
    CanSwing = true;
}

void UMeleeAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (HitboxActive) 
    {
        SetHitboxLocation();
    }
}

void UMeleeAttack::OnComponentBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor != nullptr && OtherActor != GetOwner())
    {
        UEnemyStats* Enemy = Cast<UEnemyStats>(OtherActor->GetComponentByClass(UEnemyStats::StaticClass()));
        AEgg* Egg = Cast<AEgg>(OtherActor);

        if (OtherActor && OtherComp && Enemy && !HitEnemies.Contains(OtherActor))
        {
            // Add the enemy actor to the array of hit enemies
            HitEnemies.Add(OtherActor);

            Enemy->ChangeHealth(-MeleeDamage);

            if(Enemy->CurrentHealth>=MeleeDamage){
            AActor* BloodInstance = GetWorld()-> SpawnActor<AActor>(Blood, OtherActor->GetActorLocation(),OtherActor->GetActorRotation());

            FVector Forward = this->GetOwner()->GetActorLocation() - BloodInstance->GetActorLocation();
            FRotator Rot = UKismetMathLibrary::MakeRotFromXZ(Forward, FVector(0,0,1));
            BloodInstance->SetActorRotation(Rot);
            }
        }
        else if (OtherActor && Egg && !HitEggs.Contains(OtherActor))
        {
            // Add the egg actor to the array of hit eggs
            HitEggs.Add(OtherActor);

            Egg->ChangeHealth(-MeleeDamage);
        }
    }
}


