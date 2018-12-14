// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "ActorPool.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ATile::SetPool(UActorPool* InPool)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] Setting Pool %s"), *(this->GetName()), *(InPool->GetName()));
	Pool = InPool;
}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawnAttempts, int MaxSpawnAttempts, float Radius, float MinScale, float MaxScale)
{
	int NumberToSpawn = FMath::RandRange(MinSpawnAttempts, MaxSpawnAttempts);
	for(size_t i = 0; i < NumberToSpawn; i++)
	{
		FVector OutSpawnPoint;
		float RandomScale = FMath::RandRange(MinScale, MaxScale);
		bool Found = AttemptFindEmptyLocation(OutSpawnPoint, Radius * RandomScale);
		if(Found)
		{
			float RandomRotation = FMath::RandRange(-180.0f, 180.0f);
			PlaceActor(ToSpawn, OutSpawnPoint, RandomRotation, RandomScale);
		}
	}
}

bool ATile::AttemptFindEmptyLocation(FVector& OutLocation, float Radius)
{
	FVector Min(0.0f, -2000.f, 50.0f);
	FVector Max(4000.0f, 2000.0f, 50.0f);
	FBox TileBounds(Min, Max);
	const int MAX_ATTEMPTS = 100;
	for(size_t i = 0; i < MAX_ATTEMPTS; i++)
	{
		FVector SpawnLocationAttempt = FMath::RandPointInBox(TileBounds);
		if(CanSpawnAtLocation(SpawnLocationAttempt, Radius))
		{
			OutLocation = SpawnLocationAttempt;
			return true;
		}
	}
	return false;
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FVector SpawnPoint, float Rotation, float Scale)
{
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
	Spawned->SetActorRelativeLocation(SpawnPoint);
	Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	Spawned->SetActorRotation(FRotator(0, Rotation, 0));
	Spawned->SetActorScale3D(FVector(Scale));
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ATile::CanSpawnAtLocation(FVector Location, float Radius)
{
	FHitResult HitResult;
	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);
	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		GlobalLocation,
		GlobalLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius)
		);
	FColor ResultColour = HasHit ? FColor::Red : FColor::Green;
	// DrawDebugCapsule(GetWorld(), GlobalLocation, 0, Radius, FQuat::Identity, ResultColour, true, 100);
	return !HasHit;
}

