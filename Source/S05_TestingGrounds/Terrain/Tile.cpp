// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "ActorPool.h"
#include "Engine/World.h"
#include "AI/NavigationSystemBase.h"
#include "NavigationSystem.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MinExtent = FVector(0.0f, -2000.f, 50.0f);
	MaxExtent = FVector(4000.0f, 2000.0f, 50.0f);

	NavigationBoundsOffset = FVector(2000, 0, 0);

}

void ATile::SetPool(UActorPool* InPool)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] Setting Pool %s"), *(this->GetName()), *(InPool->GetName()));
	Pool = InPool;

	PositionNavMeshBoundsVolume();
}

void ATile::PositionNavMeshBoundsVolume()
{
	NavMeshBoundsVolume = Pool->Checkout();

	if(NavMeshBoundsVolume == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Not enough actors in pool."), *GetName());
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("[%s] Checked out: {%s}"), *GetName(), *NavMeshBoundsVolume->GetName());
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavigationBoundsOffset);
	FNavigationSystem::Build(*GetWorld());
}

template<class T>
void ATile::RandomlyPlaceActors(TSubclassOf<T> ToSpawn, FActorSpawnInfo SpawnInfo)
{
	TArray<FSpawnPosition> SpawnPositions;
	int NumberToSpawn = FMath::RandRange(SpawnInfo.MinSpawnAttempts, SpawnInfo.MaxSpawnAttempts);
	for(size_t i = 0; i < NumberToSpawn; i++)
	{
		FSpawnPosition SpawnPosition;
		SpawnPosition.Scale = FMath::RandRange(SpawnInfo.MinScale, SpawnInfo.MaxScale);
		bool Found = AttemptFindEmptyLocation(SpawnPosition.Location, SpawnInfo.Radius * SpawnPosition.Scale);
		if(Found)
		{
			SpawnPosition.Rotation = FMath::RandRange(-180.0f, 180.0f);
			PlaceActor(ToSpawn, SpawnPosition);
		}
	}
}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, FActorSpawnInfo SpawnInfo)
{
	RandomlyPlaceActors(ToSpawn, SpawnInfo);
}

void ATile::PlaceAIPawns(TSubclassOf<APawn> ToSpawn, FActorSpawnInfo AISpawnInfo)
{
	RandomlyPlaceActors(ToSpawn, AISpawnInfo);
}

bool ATile::AttemptFindEmptyLocation(FVector& OutLocation, float Radius)
{
	FBox TileBounds(MinExtent, MaxExtent);
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

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, const FSpawnPosition SpawnPosition)
{
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
	if(Spawned)
	{
		Spawned->SetActorRelativeLocation(SpawnPosition.Location);
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
		Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));
	}
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	Pool->Return(NavMeshBoundsVolume);
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATile::PlaceActor(TSubclassOf<APawn> ToSpawn, const FSpawnPosition SpawnPosition)
{
	APawn* Spawned = GetWorld()->SpawnActor<APawn>(ToSpawn);
	if(Spawned)
	{
		Spawned->SetActorRelativeLocation(SpawnPosition.Location);
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
		Spawned->SpawnDefaultController();
		Spawned->Tags.Add(FName("Enemy"));
	}
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
	return !HasHit;
}

