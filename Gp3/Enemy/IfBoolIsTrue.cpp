// Fill out your copyright notice in the Description page of Project Settings.

#include "IfBoolIsTrue.h"
#include "BehaviorTree/BlackboardComponent.h"

UIfBoolIsTrue::UIfBoolIsTrue()
{
	NodeName = "If bool is true";
}

bool UIfBoolIsTrue::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	return OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());
}

void UIfBoolIsTrue::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}

void UIfBoolIsTrue::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskDeactivated(Task);
}

