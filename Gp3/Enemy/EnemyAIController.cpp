// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Gp3/Environment/CoverPoint.h"

AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	if(!BehaviorTree) return;
	
	RunBehaviorTree(BehaviorTree);
	GetBlackboardComponent()->SetValueAsBool(TEXT("IsAggro"), false);
	GetBlackboardComponent()->SetValueAsBool(TEXT("IsInCover"), false);
	GetBlackboardComponent()->SetValueAsBool(TEXT("IsInCoverOfPlayer"), true);
}

void AEnemyAIController::Aggro()
{
	GetBlackboardComponent()->SetValueAsBool(TEXT("IsAggro"), true);
}
