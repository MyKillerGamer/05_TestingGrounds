// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PatrolComponent.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	// Get all patrol points from PatrolComponent
	auto ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	UPatrolComponent* PatrolComponent = ControlledPawn->FindComponentByClass<UPatrolComponent>();
	if(!ensure(PatrolComponent)) { return EBTNodeResult::Failed; }

	// warn about empty patrol routes
	auto PatrolPoints = PatrolComponent->GetPatrolPoints();
	if(PatrolPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("%f has not patrol points"), *ControlledPawn->GetName());
		return EBTNodeResult::Failed;
	}

	// Set next Waypoint

	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	if(!PatrolPoints[Index]) { return EBTNodeResult::Failed; }
	BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);

	// Cycle Index

	auto NextIndex = (Index + 1) % PatrolPoints.Num();
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);

	return EBTNodeResult::Succeeded;
}