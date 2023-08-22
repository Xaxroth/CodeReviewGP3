
#pragma once

#include "CoreMinimal.h"
#include "EnemySpawner.h"
#include "Door.h"
#include "MutationPickup.h"
#include "GameFramework/Actor.h"
#include "Encounter.generated.h"



UCLASS()
class TEAM12_API AEncounter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEncounter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateEnemyList();
	
	
	UPROPERTY(VisibleAnywhere , BlueprintReadWrite)
	class UBoxComponent* CollisionBox;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent*  OverLappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp , int32 OtherBody ,bool bFromSweep,const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void StartEncounter();
	void FinishEncounter();
	void SpawnWave();
	
	UPROPERTY(EditAnywhere ,Category="Enemies")
	TArray<AEnemySpawner*> Spawners;

	UPROPERTY(EditAnywhere ,Category="Enemies")
	int EnemiesLeft;

	UPROPERTY(EditAnywhere ,Category="Enemies")
	TArray<AActor*> EnemyInstances;
	
	UPROPERTY(VisibleAnywhere,Category="Doors")
	ADoor* EntranceDoor;
	
	UPROPERTY(VisibleAnywhere,Category="Doors")
	ADoor* ExitDoor;

	UFUNCTION(BlueprintImplementableEvent)
	void LeaveRoom();

	UPROPERTY(EditAnywhere,Category="Mutations")
	TSubclassOf<AMutationPickup> MutationPickUp;
	
	UPROPERTY(EditAnywhere,Category="Mutations")
    FVector PickUpLocalPosition;

	UFUNCTION(BlueprintImplementableEvent)
	void EncounterIsOver();


		//0: was never started , 1 is currently starting, 2 is over
		int EncounterState = 0;


	////// Timer Related Stuff //////
		int32 milliseconds;
    	int32 seconds;
    	int32 minutes;
		int counter;
		ATeam12GameMode* GameMode;
		float TotalTimeElapsed;
		void ReturnTime();
		
		UFUNCTION(BlueprintCallable)
		int ReturnAmountOfEnemies();

	UPROPERTY(VisibleAnywhere, Category="Timer")
		FString SavedTime;

	bool LeftRoom;
 	
private:

	bool isInEncounter;
	

};
