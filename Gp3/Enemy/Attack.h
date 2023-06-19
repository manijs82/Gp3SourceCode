// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Attack.generated.h"

UCLASS()
class GP3_API UAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	FTimerHandle FirstShotTimerHandle;

public:
	UAttack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;
	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override;

	bool bFirstTimeShooting = true;
	void FirstTimeShootingCooldown();
};
