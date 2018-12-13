// Fill out your copyright notice in the Description page of Project Settings.

#include "Rifle.h"
#include "../Weapons/BallProjectile.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ARifle::ARifle()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	SetRootComponent(FP_Gun);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));
}

// Called when the game starts or when spawned
void ARifle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARifle::OnFire()
{
	// try and fire a projectile
	if(ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if(World != nullptr)
		{
			const FRotator SpawnRotation = FP_MuzzleLocation->GetComponentRotation();
			const FVector SpawnLocation = FP_MuzzleLocation->GetComponentLocation();

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			World->SpawnActor<ABallProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}

	// try and play the sound if specified
	if(FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
		// Get the animation object for the arms mesh
	if(FireAnimation1P != nullptr && AnimInstance1P != nullptr)
	{
		AnimInstance1P->Montage_Play(FireAnimation1P, 1.f);
	}

	// try and play a firing animation if specified
		// Get the animation object for the arms mesh
	if(FireAnimation3P != nullptr && AnimInstance3P != nullptr)
	{
		AnimInstance3P->Montage_Play(FireAnimation3P, 1.f);
	}
}