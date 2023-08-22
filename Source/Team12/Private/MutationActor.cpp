// Fill out your copyright notice in the Description page of Project Settings.


#include "MutationActor.h"
#include "../Team12Character.h"

AMutationActor::AMutationActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMutationActor::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	ACharacter* FirstPersonCharacter = PlayerController->GetCharacter();

	if (FirstPersonCharacter != nullptr)
	{
		PlayerAttributes = Cast<UPlayerStats>(FirstPersonCharacter->GetComponentByClass(UPlayerStats::StaticClass()));
	}

	if (PlayerAttributes != nullptr)
	{
			// Which attribute should this mutation strain change?
			switch (Type)
			{
				case MutationActorType::None:
					return;
					break;
				case MutationActorType::MaxDamage:
					VariableToChange = PlayerAttributes->MeleeDamage;
					break;
				case MutationActorType::MaxHealth:
					VariableToChange = PlayerAttributes->MaxHealth;
					break;
				case MutationActorType::MaxSpeed:
					VariableToChange = PlayerAttributes->MovementSpeed;
					break;
				case MutationActorType::DashDamage:
					VariableToChange = PlayerAttributes->DashDamage;
					break;
				case MutationActorType::DashCooldown:
					VariableToChange = PlayerAttributes->DashCoolDown;
					break;
				case MutationActorType::DashDistance:
					VariableToChange = PlayerAttributes->DashDistance;
					break;
				case MutationActorType::DashCharges:
					VariableToChange = PlayerAttributes->DashCharges;
					break;
				case MutationActorType::FieldOfView:
					VariableToChange = PlayerAttributes->FieldOfView;
					break;
				case MutationActorType::HookshotRange:
					VariableToChange = PlayerAttributes->HookshotRange;
					break;
				case MutationActorType::HookshotCooldown:
					VariableToChange = PlayerAttributes->HookshotCooldown;
					break;
				case MutationActorType::HookshotCharges:
					VariableToChange = PlayerAttributes->HookshotCharges;
					break;
				case MutationActorType::MeleeRange:
					VariableToChange = PlayerAttributes->MeleeRange;
					break;
				case MutationActorType::JumpHeight:
					VariableToChange = PlayerAttributes->JumpForce;
					break;
				case MutationActorType::CanDoubleJump:
					PlayerAttributes->DoubleJump = true;
					break;
				case MutationActorType::MaxShots:
					VariableToChange = PlayerAttributes->MaxShots;
					break;
				case MutationActorType::ShootingCoolDown:
					VariableToChange = PlayerAttributes->ShootingCoolDown;
					break;
				case MutationActorType::GunDamage:
					VariableToChange = PlayerAttributes->GunDamage;
					break;
			}

			// How should the attribute be affected mathematically?
			switch (Effect)
			{
				case MutationActorEffect::None:
					return;
					break;
				case MutationActorEffect::Add:
					VariableToChange += ChangeFloatValue;
					break;
				case MutationActorEffect::Subtract:
					VariableToChange -= ChangeFloatValue;
					break;
				case MutationActorEffect::Multiply:
					VariableToChange *= ChangeFloatValue;
					break;
				case MutationActorEffect::Divide:
					VariableToChange /= ChangeFloatValue;
					break;
			}

			// Is the mutation strain legendary? If yes, which power should the legendary effect give ontop of its ordinary stats?
			if (Rarity == MutationActorRarity::Legendary)
			{
				switch (LegendaryEffect)
				{
					case LegendaryMutationAbility::None:
						break;
					case LegendaryMutationAbility::DoubleJump:
						PlayerAttributes->DoubleJump = true;
						break;
				}
			}

			// If the end sum is less than 0.1f of a specific attribute, this will prevent that attribute from ever falling under this threshold.
			if (VariableToChange <= 0.1f) 
			{
				VariableToChange = 0.1f;
			}

			// Checks which type this mutation type is, and sets the current attribute to the new value.
			switch (Type)
			{
				case MutationActorType::None:
					return;
					break;
				case MutationActorType::MaxDamage:
					PlayerAttributes->MeleeDamage = VariableToChange;
					break;
				case MutationActorType::MaxHealth:
					PlayerAttributes->MaxHealth = VariableToChange;
					break;
				case MutationActorType::MaxSpeed:
					PlayerAttributes->MovementSpeed = VariableToChange;
					break;
				case MutationActorType::DashDamage:
					PlayerAttributes->DashDamage = VariableToChange;
					break;
				case MutationActorType::DashCooldown:
					PlayerAttributes->DashCoolDown = VariableToChange;
					break;
				case MutationActorType::DashDistance:
					PlayerAttributes->DashDistance = VariableToChange;
					break;
				case MutationActorType::DashCharges:
					PlayerAttributes->DashCharges = VariableToChange;
					break;
				case MutationActorType::FieldOfView:
					PlayerAttributes->FieldOfView = VariableToChange;
					break;
				case MutationActorType::HookshotRange:
					PlayerAttributes->HookshotRange = VariableToChange;
					break;
				case MutationActorType::HookshotCooldown:
					PlayerAttributes->HookshotCooldown = VariableToChange;
					break;
				case MutationActorType::HookshotCharges:
					PlayerAttributes->HookshotCharges = (int)VariableToChange;
					break;
				case MutationActorType::MeleeRange:
					PlayerAttributes->MeleeRange = VariableToChange;
					break;
				case MutationActorType::JumpHeight:
					PlayerAttributes->JumpForce = VariableToChange;
					break;
				case MutationActorType::CanDoubleJump:
					PlayerAttributes->DoubleJump = true;
					break;
				case MutationActorType::MaxShots:
					VariableToChange = PlayerAttributes->MaxShots;
					break;
				case MutationActorType::ShootingCoolDown:
					VariableToChange = PlayerAttributes->ShootingCoolDown;
					break;
				case MutationActorType::GunDamage:
					VariableToChange = PlayerAttributes->GunDamage;
					break;
			}

			// Update the player stats during runtime.
			if (FirstPersonCharacter != nullptr)
			{
				ATeam12Character* Player = Cast<ATeam12Character>(GetOwner());

				if (Player != nullptr)
				{
					Player->SetNewValuesBody();
					Player->SetNewValuesMovement();
				}
			}
	}
}


void AMutationActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

