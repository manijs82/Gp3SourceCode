﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "UpdatePlayerLocation.generated.h"

class AEnemyBase;

UCLASS()
class GP3_API UUpdatePlayerLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UUpdatePlayerLocation();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;
	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override;
};
