// Fill out your copyright notice in the Description page of Project Settings.


#include "Encounter.h"

#include "PlayerFollower.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Team12/Team12Character.h"


// Sets default values
AEncounter::AEncounter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	CollisionBox -> SetBoxExtent(FVector(32.f,32.f,32.f));
	CollisionBox -> SetCollisionProfileName("Trigger");
	RootComponent = CollisionBox;
    
	CollisionBox-> OnComponentBeginOverlap.AddDynamic(this , &AEncounter::OnOverlapBegin);
	    
	CollisionBox-> OnComponentEndOverlap.AddDynamic(this , &AEncounter::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AEncounter::BeginPlay()
{
	Super::BeginPlay();


	GameMode = Cast<ATeam12GameMode>(GetWorld()->GetAuthGameMode());
	
}

// Called every frame
void AEncounter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if(EnemyInstances.Num() ==0 && isInEncounter && EnemiesLeft>0)
	{
		GEngine -> AddOnScreenDebugMessage(-1 , 1.f , FColor:: Black, "Starting A new Wave");
		SpawnWave();
	}else if(EnemyInstances.Num() ==0 && EnemiesLeft == 0&& isInEncounter)
	{
	FinishEncounter();
		
	}


	if(EncounterState ==1){
	if (!GameMode->PauseTimer) 
	{
		TotalTimeElapsed += DeltaTime;


		milliseconds += (10000/ (1/ DeltaTime))  ;
		counter++;
		if(counter>= 1/ DeltaTime)
		{
			seconds++;
			
			milliseconds =0;
			counter=0;
		}
		int32 DisplayedMilliseconds = milliseconds/100;

		if(seconds>59)
		{
			minutes++;
			seconds=0;
		}
	
	}
	}else if(EncounterState == 2)
	{
		ReturnTime();
		EncounterState =3;
	}
}

void AEncounter::OnOverlapBegin(UPrimitiveComponent*  OverLappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp , int32 OtherBody ,bool bFromSweep,const FHitResult& SweepResult)
{

	ATeam12Character* player= Cast<ATeam12Character>(OtherActor);



	if (player && !isInEncounter && EnemiesLeft>0 )
	{
		StartEncounter();
	}

	
}


void AEncounter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerFollower* player= Cast<APlayerFollower>(OtherActor);

	
	if (player)
	{
		LeftRoom=true;
		LeaveRoom();
		GEngine -> AddOnScreenDebugMessage(-1 , 10.f , FColor:: Blue, "ran Away");

	}
}
void AEncounter::StartEncounter()
{
	GEngine -> AddOnScreenDebugMessage(-1 , 10.f , FColor:: Blue, "Get Ready to Rumble!!!");

	isInEncounter = true;
	SpawnWave();

	if(EntranceDoor){
	EntranceDoor->CloseDoor();
	}
	if(ExitDoor){
		ExitDoor->CloseDoor();
	}
		EncounterState=1;
}

void AEncounter::SpawnWave()
{
	for(int i = 0 ; i< Spawners.Num(); i++)
	{
		if(EnemiesLeft>0){
			Spawners[i]->SpawnEnemy();
			EnemiesLeft--;
		}else
		{
			FinishEncounter();
			break;
		}
	}
}


void AEncounter::FinishEncounter()
{
	GEngine -> AddOnScreenDebugMessage(-1 , 10.f , FColor:: Blue, "It's over!!!");

	EncounterIsOver();
	
	isInEncounter=false;

	if(ExitDoor){
	ExitDoor->OpenDoor();
	}
  	AActor* Current = GetWorld()-> SpawnActor<AActor>(MutationPickUp , this->GetActorLocation()+PickUpLocalPosition ,this->GetActorRotation());
	
	EncounterState = 2;
}
void AEncounter:: ReturnTime()
{

	SavedTime.AppendInt(minutes);
	SavedTime.Append(":");
	SavedTime.AppendInt(seconds);
	SavedTime.Append(":");
	SavedTime.AppendInt(milliseconds);
	GameMode->LastEncounterTime = SavedTime;
	
	GEngine -> AddOnScreenDebugMessage(-1 , 10.f , FColor:: Blue, SavedTime);

}
int AEncounter:: ReturnAmountOfEnemies()
{
	return EnemyInstances.Num();
}
