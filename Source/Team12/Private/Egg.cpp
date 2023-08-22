// Fill out your copyright notice in the Description page of Project Settings.


#include "Egg.h"
#include "../Team12Character.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AEgg::AEgg()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEgg::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (PlayerCharacter)
	{
		PlayerStatsComponent = PlayerCharacter->FindComponentByClass<UPlayerStats>();
	}

	ATeam12GameMode* temp = Cast<ATeam12GameMode>(GetWorld()->GetAuthGameMode());
	temp->TotalEggs++;
}

void AEgg::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEgg::ChangeHealth(float Value) 
{
	EggHealth += Value;

	if (EggHealth <= 0 && PlayerStatsComponent)
	{
		PlayerStatsComponent->TemporarySpeedIncrease += SpeedIncreaseAmount;
		PlayerStatsComponent->HasSpeedBuff = true;
		PlayerStatsComponent->TimeUntilBuffWanes = 10.f;
		PlayerStatsComponent->MovementSpeed += SpeedIncreaseAmount;
		ATeam12Character* PlayerCharacter = Cast<ATeam12Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		if (PlayerCharacter)
		{
			PlayerCharacter->SetNewValuesMovement();
			PlayerCharacter->UpdatePlayerStats();
			PlayerStatsComponent->ChangeHealth(HealAmount);

		}

		ATeam12GameMode* GameMode = Cast<ATeam12GameMode>(GetWorld()->GetAuthGameMode());
		GameMode->EggsDestroyed++;
		Destroy();
	}
}

