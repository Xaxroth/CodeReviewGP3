// Fill out your copyright notice in the Description page of Project Settings.


#include "MutationCluster.h"
#include "../Team12Character.h"
#include "../Team12GameMode.h"

// Sets default values
AMutationCluster::AMutationCluster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AMutationCluster::BeginPlay()
{
	Super::BeginPlay();
}

void AMutationCluster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(WasChosen)
	{
		Unpack();
	}
}

void AMutationCluster::Unpack()
{
	if (Mutations.Num() > 0)
	{
		for (size_t i = 0; i < Mutations.Num(); i++)
		{
			// Retrieve the random element
			TSubclassOf<AActor> ActorClass = Mutations[i];
			AMutationActor* Mutation = Cast<AMutationActor>(ActorClass.GetDefaultObject());

			if (Mutation != nullptr) 
			{
				FString ActorName = ActorClass->GetName();

				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = Player;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				GetWorld()->SpawnActor<AMutationActor>(ActorClass, FTransform::Identity, SpawnParams);

				Mutation->AttachToComponent(Player->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
				Mutation->SetActorRelativeLocation(FVector::ZeroVector);
				Mutation->SetActorRelativeRotation(FRotator::ZeroRotator);
			}
		}
	}

	if (RemoveFromMutationPoolOnPickup) 
	{
		ATeam12GameMode* GameMode = GetWorld()->GetAuthGameMode<ATeam12GameMode>();
		if (GameMode)
		{
			GameMode->Mutations.Remove(TSubclassOf<AActor>(this->GetClass()));
		}
	}

	Destroy();
}
