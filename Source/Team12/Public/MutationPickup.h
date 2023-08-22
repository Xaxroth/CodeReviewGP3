// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MutationActor.h"
#include "MutationCluster.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
//#include "Perception/AIPerceptionTypes.h"
#include "Team12/Team12Character.h"
#include "Team12/Team12GameMode.h"
#include "MutationPickup.generated.h"

UCLASS()
class TEAM12_API AMutationPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMutationPickup();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collider")
	UBoxComponent* CollisionComponent;


	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);



	UPROPERTY(VisibleAnywhere, Category= "Saved Mutations")
	AMutationCluster*  Option1;

	UPROPERTY(VisibleAnywhere, Category= "Saved Mutations")
	AMutationCluster* Option2;

	UFUNCTION(BlueprintCallable)
	void Option1ReferencePlayer(ACharacter* player);
	UFUNCTION(BlueprintCallable)
	void Option2ReferencePlayer(ACharacter* player);

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GenerateRandomMutation();
	void AttachMutation(AMutationCluster* AttachedMutation,  TSubclassOf<AActor> UsedActor ,FActorSpawnParameters SpawnParams , ATeam12Character* Player);
	void CheckIfShouldBeRemoved(AMutationCluster* , ATeam12GameMode* GameMode , int RandomIndex);


	UFUNCTION(BlueprintCallable , Category ="Return")
	void ReturnOption1();
	UFUNCTION(BlueprintCallable , Category ="Return")
	void ReturnOption2();

	UFUNCTION(BlueprintCallable , Category ="Return")
	void ReturnOption1Data(FString& Name, FString& Description , FString& Rarity );
	UFUNCTION(BlueprintCallable , Category ="Return")
	void ReturnOption2Data(FString& Name, FString& Description , FString& Rarity );

	UFUNCTION(BlueprintImplementableEvent,  Category = "Begin")
	void BeginMutationChoice();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
