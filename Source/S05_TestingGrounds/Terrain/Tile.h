// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

USTRUCT(BlueprintType)
struct FSpawnPosition
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FVector Location;
	UPROPERTY()
	float Rotation;
	UPROPERTY()
	float Scale;
};

USTRUCT(BlueprintType)
struct FActorSpawnInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int MinSpawnAttempts;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int MaxSpawnAttempts;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float MinScale;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float MaxScale;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float Radius;

	FActorSpawnInfo()
	{
		MinSpawnAttempts = 1;
		MaxSpawnAttempts = 1;
		MinScale = 1;
		MaxScale = 1;
		Radius = 500;
	}
};

UCLASS()
class S05_TESTINGGROUNDS_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	UFUNCTION(BlueprintCallable, Category = "Generation")
	void PlaceActors(TSubclassOf<AActor> ToSpawn, FActorSpawnInfo SpawnInfo);

	UFUNCTION(BlueprintCallable, Category = "Generation")
	void PlaceAIPawns(TSubclassOf<APawn> ToSpawn, FActorSpawnInfo SpawnInfo);

	TArray<FSpawnPosition> RandomSpawnPositions(int MinSpawnAttempts, int MaxSpawnAttempts, float MinScale, float MaxScale, float Radius);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(	const EEndPlayReason::Type EndPlayReason	) override;

	UPROPERTY(EditDefaultsOnly, Category="Spawning")
	FVector MinExtent;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MaxExtent;

	UPROPERTY(EditDefaultsOnly, Category = "Navigation")
	FVector NavigationBoundsOffset;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Pool")
	void SetPool(class UActorPool* Pool);

private:

	void PositionNavMeshBoundsVolume();

	bool AttemptFindEmptyLocation(FVector& OutLocation, float Radius);

	template<class T>
	void RandomlyPlaceActors(TSubclassOf<T> ToSpawn, FActorSpawnInfo SpawnInfo);

	void PlaceActor(TSubclassOf<AActor> ToSpawn, const FSpawnPosition SpawnPosition);
	
	void PlaceActor(TSubclassOf<APawn> ToSpawn, const FSpawnPosition SpawnPosition);

	bool CanSpawnAtLocation(FVector Location, float Radius);

	UActorPool* Pool;

	AActor* NavMeshBoundsVolume;
};