// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyUncrouch.h"
#include "EnemyBase.h"
#include "BehaviorTree/BlackboardComponent.h"

UEnemyUncrouch::UEnemyUncrouch()
{
	NodeName = "Uncrouch";
}

EBTNodeResult::Type UEnemyUncrouch::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	TObjectPtr<AEnemyBase> MyEnemy = Cast<AEnemyBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("SelfActor")));
	MyEnemy->EnemyUnCrouch();
	
	return EBTNodeResult::Succeeded;
}

void UEnemyUncrouch::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}

void UEnemyUncrouch::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskDeactivated(Task);
}

