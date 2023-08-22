// Fill out your copyright notice in the Description page of Project Settings.


#include "MutationPickup.h"
#include "MutationActor.h"
#include "MutationCluster.h"
#include "Mutation.h"
#include "Math/RandomStream.h"
#include "../Team12Character.h"
#include "../Team12GameMode.h"

AMutationPickup::AMutationPickup()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Component"));
    SetRootComponent(CollisionComponent);


	CollisionComponent-> OnComponentBeginOverlap.AddDynamic(this , &AMutationPickup::OnOverlapBegin);
	
}

void AMutationPickup::BeginPlay()
{
	Super::BeginPlay();


	ATeam12GameMode* GameMode = GetWorld()->GetAuthGameMode<ATeam12GameMode>();
	if (GameMode != nullptr)
	{
		if (GameMode->Mutations.Num() > 0)
		{
			// Generate a random index within the array bounds
			int32 RandomIndex1 = FMath::RandRange(0, GameMode->Mutations.Num() - 1);
			int32 RandomIndex2 = FMath::RandRange(0, GameMode->Mutations.Num() - 1);

			// make sure they're different 
			if(RandomIndex1 == RandomIndex2)
			{
				RandomIndex2= FMath::RandRange(0, GameMode->Mutations.Num() - 1);

                    
				// Try again if they are still the same
				if(RandomIndex1 == RandomIndex2)
				{
					RandomIndex2= FMath::RandRange(0, GameMode->Mutations.Num() - 1);
					//If they're STILL the same , give up
				}
			}
			// Retrieve the random element
			Option1 = GetWorld()->SpawnActor<AMutationCluster>(GameMode->Mutations[RandomIndex1], FTransform::Identity);;
			Option2 =  GetWorld()->SpawnActor<AMutationCluster>(GameMode->Mutations[RandomIndex2], FTransform::Identity);
		}
			
	}
}

void AMutationPickup::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	/*CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMutationPickup::OnOverlapBegin);


	ATeam12Character* Player = Cast<ATeam12Character>(OtherActor);
	Option1->Player = Player;
	Option2->Player = Player;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Collided!"));
	*/

//	BeginMutationChoice();
} 

void AMutationPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/*
void AMutationPickup::AttachMutation(AMutationCluster* AttachedMutation,  TSubclassOf<AActor> UsedActor ,FActorSpawnParameters SpawnParams, ATeam12Character* Player)
{
	AMutationCluster* Current = GetWorld()-> SpawnActor<AMutationCluster>(UsedActor ,FTransform::Identity,SpawnParams);
	
    if (Current != nullptr)
    {
        AttachedMutation->AttachToComponent(Player->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        AttachedMutation->SetActorRelativeLocation(FVector::ZeroVector);
        AttachedMutation->SetActorRelativeRotation(FRotator::ZeroRotator);
    }
}


void AMutationPickup::CheckIfShouldBeRemoved(AMutationCluster* MutationActor , ATeam12GameMode* GameMode , int RandomIndex)
{
	
	if (MutationActor != nullptr && MutationActor->RemoveFromMutationPoolOnPickup)
	{
		GameMode->Mutations.RemoveAt(RandomIndex);
	}
}
*/




void AMutationPickup::ReturnOption1()
{
	Option1->WasChosen = true;
	Option2->Destroy();
	Destroy();
}
void AMutationPickup::ReturnOption2()
{
	Option2->WasChosen = true;
	Option1->Destroy();
	Destroy();
}

void AMutationPickup::ReturnOption1Data(FString& Name, FString& Description , FString& Rarity )
{
	Name = Option1->MutationClusterName;
	Description  = Option1 ->MutationDescription;
	Rarity = Option1 ->Rarity;
}


void AMutationPickup::ReturnOption2Data(FString& Name, FString& Description , FString& Rarity )
{
	Name = Option2->MutationClusterName;
	Description  = Option2 ->MutationDescription;
	Rarity = Option2 ->Rarity;
}

void AMutationPickup::Option1ReferencePlayer(ACharacter* player)
{
	Option1->Player = 	player;
}

void AMutationPickup::Option2ReferencePlayer(ACharacter* player)
{
	Option2->Player = 	player;
}