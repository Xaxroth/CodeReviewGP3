// Fill out your copyright notice in the Description page of Project Settings.


#include "Mutation.h"
#include "../Team12Character.h"
// Sets default values for this component's properties
UMutation::UMutation()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMutation::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	// Get the character from the player controller
	ACharacter* FirstPersonCharacter = PlayerController->GetCharacter();

	// Check if the character is valid
	if (FirstPersonCharacter != nullptr)
	{
		PlayerAttributes = Cast<UPlayerStats>(FirstPersonCharacter->GetComponentByClass(UPlayerStats::StaticClass()));
	}

	if (PlayerAttributes != nullptr) 
	{
		if (GetOwner() && GetOwner()->IsA<ATeam12Character>())
		{
			switch (Type) 
			{
				case MutationType::None:
					return;
					break;
				case MutationType::MaxDamage:
					VariableToChange = PlayerAttributes->MeleeDamage;
					break;
				case MutationType::MaxHealth:
					VariableToChange = PlayerAttributes->MaxHealth;
					break;
				case MutationType::MaxSpeed:
					VariableToChange = PlayerAttributes->MovementSpeed;
					break;
				case MutationType::DashDamage:
					VariableToChange = PlayerAttributes->MovementSpeed;
					break;
				case MutationType::DashCooldown:
					VariableToChange = PlayerAttributes->DashCoolDown;
					break;
				case MutationType::DashDistance:
					VariableToChange = PlayerAttributes->DashDistance;
					break;
				case MutationType::DashCharges:
					VariableToChange = PlayerAttributes->DashCharges;
					break;
				case MutationType::FieldOfView:
					VariableToChange = PlayerAttributes->FieldOfView;
					break;
				case MutationType::HookshotRange:
					VariableToChange = PlayerAttributes->HookshotRange;
					break;
				case MutationType::HookshotCooldown:
					VariableToChange = PlayerAttributes->HookshotCooldown;
					break;
				case MutationType::HookshotCharges:
					VariableToChange = PlayerAttributes->HookshotCharges;
					break;
				case MutationType::MeleeRange:
					VariableToChange = PlayerAttributes->MeleeRange;
					break;
				case MutationType::JumpHeight:
					VariableToChange = PlayerAttributes->JumpForce;
					break;
				case MutationType::MaxShots:
					VariableToChange = PlayerAttributes->MaxShots;
					break;
				case MutationType::ShootingCoolDown:
					VariableToChange = PlayerAttributes->ShootingCoolDown;
					break;
				case MutationType::GunDamage:
					VariableToChange = PlayerAttributes->GunDamage;
					break;
				case MutationType::CanDoubleJump:
				PlayerAttributes->DoubleJump = true;
				break;
			}
			if (Rarity != MutationRarity::Legendary) 
			{
				switch (Effect) 
				{
					case MutationEffect::None:
						return;
						break;
					case MutationEffect::Add:
						VariableToChange += ChangeFloatValue;
						break;
					case MutationEffect::Subtract:
						VariableToChange -= ChangeFloatValue;
						break;
					case MutationEffect::Multiply:
						VariableToChange *= ChangeFloatValue;
						break;
					case MutationEffect::Divide:
						VariableToChange /= ChangeFloatValue;
						break;
				}
			}

			switch (Type)
			{
				case MutationType::None:
					return;
					break;
				case MutationType::MaxDamage:
					PlayerAttributes->MeleeDamage = VariableToChange;
					break;
				case MutationType::MaxHealth:
					PlayerAttributes->MaxHealth = VariableToChange;
					break;
				case MutationType::MaxSpeed:
					PlayerAttributes->MovementSpeed = VariableToChange;
					break;
				case MutationType::DashDamage:
					PlayerAttributes->DashDamage = VariableToChange;
					break;
				case MutationType::DashCooldown:
					PlayerAttributes->DashCoolDown = VariableToChange;
					break;
				case MutationType::DashDistance:
					PlayerAttributes->DashDistance = VariableToChange;
					break;
				case MutationType::DashCharges:
					PlayerAttributes->DashCharges = VariableToChange;
					
					break;
				case MutationType::FieldOfView:
					PlayerAttributes->FieldOfView = VariableToChange;
					break;
				case MutationType::HookshotRange:
					PlayerAttributes->HookshotRange = VariableToChange;
					break;
				case MutationType::HookshotCooldown:
					PlayerAttributes->HookshotCooldown = VariableToChange;
					break;
				case MutationType::HookshotCharges:
					PlayerAttributes->HookshotCharges = (int)VariableToChange;
					break;
				case MutationType::MeleeRange:
					PlayerAttributes->MeleeRange = VariableToChange;
					break;
				case MutationType::JumpHeight:
					PlayerAttributes->JumpForce = VariableToChange;
					break;
				case MutationType::MaxShots:
					VariableToChange = PlayerAttributes->MaxShots;
					break;
				case MutationType::ShootingCoolDown:
					VariableToChange = PlayerAttributes->ShootingCoolDown;
					break;
				case MutationType::GunDamage:
					VariableToChange = PlayerAttributes->GunDamage;
					break;
			case MutationType::CanDoubleJump:
				PlayerAttributes->DoubleJump = true;
				break;
			}
			switch (LegendaryEffect) 
			{
			case LegendaryMutations::None:
				return;
				break;
			case LegendaryMutations::DoubleJump:
				PlayerAttributes->DoubleJump = true;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("Picked up DoubleJump")));
				break;
			}

			if (IncreasesMutationLevel) 
			{
				PlayerAttributes->MutationLevel += MutationAmount;
			}
			
		}
	}


	ATeam12Character* player = Cast<ATeam12Character>(PlayerAttributes->GetOwner());

	player-> SetNewValuesBody();
	player-> SetNewValuesMovement();
}


// Called every frame
void UMutation::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

