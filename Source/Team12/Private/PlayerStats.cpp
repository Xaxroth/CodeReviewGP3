// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStats.h"
#include "Kismet/GameplayStatics.h"
#include "Team12/Team12Character.h"

// Sets default values for this component's properties
UPlayerStats::UPlayerStats()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UPlayerStats::BeginPlay()
{
	Super::BeginPlay();

	Stamina= MaxStamina;
	Health = MaxHealth;
}


// Called every frame
void UPlayerStats::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TimeUntilBuffWanes > 0) 
	{
		TimeUntilBuffWanes -= DeltaTime;
	}
	else 
	{
		ATeam12Character* PlayerCharacter = Cast<ATeam12Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		if (PlayerCharacter && HasSpeedBuff)
		{
			PlayerStatsComponent = PlayerCharacter->FindComponentByClass<UPlayerStats>();
			PlayerStatsComponent->MovementSpeed -= PlayerStatsComponent->TemporarySpeedIncrease;
			PlayerStatsComponent->TemporarySpeedIncrease = 0.f;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("UPDATING PLAYER STATS FROM PLAYERSTATS SCRIPT!")));
			PlayerStatsComponent->HasSpeedBuff = false;
			PlayerCharacter->UpdatePlayerSpeed();
			PlayerCharacter->UpdatePlayerStats();
		}
	}

}

bool UPlayerStats::DashState(bool IsAvailable)
{
	
	return IsAvailable;
	
}


float UPlayerStats::ReturnDashDamage( )
{

	return DashDamage;	
}

float UPlayerStats::ReturnDashDistance()
{
	return DashDistance;
}

float UPlayerStats::ReturnHookshotCooldown(float Cooldown) 
{
	return HookshotCooldown;
}


void UPlayerStats::ChangeHealth(float ChangeByThis)
{
	Health += ChangeByThis;

	UGameEndDataTracker* DataTracker = Cast<UGameEndDataTracker>(GetOwner()->GetComponentByClass(UGameEndDataTracker::StaticClass()));
	if(ChangeByThis <0)
	{
		DataTracker-> GameMode->HPLost += ChangeByThis;	
	}

	if(Health >MaxHealth)
	{
		Health=MaxHealth;
	}
	ATeam12Character* PlayerCharacter = Cast<ATeam12Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	PlayerCharacter->ChangedHP();	
}

float UPlayerStats::ReturnHealth()
{
	return Health;
}

void UPlayerStats::ChangeStamina(float ChangeByThis)
{
	Stamina += ChangeByThis;

}


float UPlayerStats::ReturnStamina()
{
	return Stamina;
}	


float UPlayerStats::ReturnDashCoolDown()
{
	return DashCoolDown;
}

