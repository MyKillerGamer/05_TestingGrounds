// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorPool.h"

// Sets default values for this component's properties
UActorPool::UActorPool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

AActor * UActorPool::Checkout()
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] Checkout"), *GetName());
	return nullptr;
}

void UActorPool::Return(AActor * ActorToReturn)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] Actor Returned: {%s}"), *GetName(), *ActorToReturn->GetName());
}

void UActorPool::Add(AActor * ActorToAdd)
{
}
