// Fill out your copyright notice in the Description page of Project Settings.


#include "SpitProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "../Team12Character.h"

// Sets default values
ASpitProjectile::ASpitProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASpitProjectile::BeginPlay()
{
	Super::BeginPlay();

    UBoxComponent* BoxComponent = Cast<UBoxComponent>(GetComponentByClass(UBoxComponent::StaticClass()));
	if (BoxComponent)
	{
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ASpitProjectile::OnBeginOverlap);
	}

	UStaticMeshComponent* ChildRoot = FindComponentByClass<UStaticMeshComponent>();
	if (ChildRoot)
	{
		float minScale = 0.1f;   // Minimum scale factor
		float maxScale = 0.5f;   // Maximum scale factor

		float randomScaleFactor = FMath::FRandRange(minScale, maxScale);

		ChildRoot->SetWorldScale3D(FVector(randomScaleFactor * 7.f, randomScaleFactor, randomScaleFactor));

		UPrimitiveComponent* ProjectileChildComponent = Cast<UPrimitiveComponent>(ChildRoot);
		if (ProjectileChildComponent)
		{
			FVector ForceDirection = GetActorForwardVector() + (FVector(0.0f, 0.0f, 1.0f) * VerticalSpeed);

			ChildRoot->AddForce(ForceDirection * ProjectileForce, NAME_None, true);
		}
	}
}

void ASpitProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(ATeam12Character::StaticClass()) && !HasDealtDamage)
	{
		UPlayerStats* PlayerStats = Cast<UPlayerStats>(OtherActor->GetComponentByClass(UPlayerStats::StaticClass()));
		if (PlayerStats)
		{
			PlayerStats->ChangeHealth(-ProjectileDamage);
			HasDealtDamage = true;
		}
	}
	else
	{
		if ((OtherActor && OtherActor->GetRootComponent() && !OtherActor->GetRootComponent()->ComponentHasTag("Enemy") && !OtherActor->GetRootComponent()->ComponentHasTag("Projectile") && !OtherActor->GetRootComponent()->ComponentHasTag("Encounter")))
		{
			Destroy();
		}
	}
}

// Called every frame
void ASpitProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector velocityVector = GetVelocity();

	float yawAngle = FMath::Atan2(velocityVector.Y, velocityVector.X);
	FQuat rotationQuat = FQuat(FVector::UpVector, yawAngle);

	SetActorRotation(rotationQuat);

	LifeTime -= DeltaTime;

	if (LifeTime <= 0) 
	{
		Destroy();
	}

}

