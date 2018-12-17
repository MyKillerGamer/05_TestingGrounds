// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class S05_TESTINGGROUNDS_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	UFUNCTION(BlueprintCallable, Category = "Generation")
	void PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawnAttempts = 1, int MaxSpawnAttempts = 1, float Radius = 500, float MinScale = 1.0f, float MaxScale = 1.0f);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(	const EEndPlayReason::Type EndPlayReason	) override;

	UPROPERTY(EditDefaultsOnly, Category="Spawning")
	FVector MinExtent;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MaxExtent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Pool")
		void SetPool(class UActorPool* Pool);

private:

	bool AttemptFindEmptyLocation(FVector& OutLocation, float Radius);
	
	void PlaceActor(TSubclassOf<AActor> ToSpawn, FVector SpawnPoint, float Rotation, float Scale);

	bool CanSpawnAtLocation(FVector Location, float Radius);

	UActorPool* Pool;

	void PositionNavMeshBoundsVolume();

	AActor* NavMeshBoundsVolume;
};
