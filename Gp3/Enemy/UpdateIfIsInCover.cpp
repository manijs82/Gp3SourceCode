// Fill out your copyright notice in the Description page of Project Settings.

#include "UpdateIfIsInCover.h"
#include "EnemyAIController.h"
#include "EnemyBase.h"
#include "BehaviorTree/BlackboardComponent.h"

UUpdateIfIsInCover::UUpdateIfIsInCover()
{
	NodeName = "Update if is in cover";
}

void UUpdateIfIsInCover::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	TObjectPtr<AEnemyBase> MyEnemy = Cast<AEnemyBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("SelfActor")));

	if(MyEnemy->GetCoverPoint())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("CoverPointLoc"), MyEnemy->GetCoverPoint()->GetActorLocation());
	}

	if(MyEnemy->IsInCover())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsInCover"), true); //Reached cover once
	}
}

void UUpdateIfIsInCover::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}

void UUpdateIfIsInCover::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskDeactivated(Task);
}
