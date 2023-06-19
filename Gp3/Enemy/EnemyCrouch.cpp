// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCrouch.h"
#include "EnemyBase.h"
#include "BehaviorTree/BlackboardComponent.h"

UEnemyCrouch::UEnemyCrouch()
{
	NodeName = "Crouch";
}

EBTNodeResult::Type UEnemyCrouch::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	TObjectPtr<AEnemyBase> MyEnemy = Cast<AEnemyBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("SelfActor")));
	MyEnemy->EnemyCrouch();
	
	return EBTNodeResult::Succeeded;
}

void UEnemyCrouch::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}

void UEnemyCrouch::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskDeactivated(Task);
}

