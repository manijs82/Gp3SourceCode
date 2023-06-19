// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

class UBehaviorTreeComponent;

UCLASS()
class GP3_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBehaviorTree> BehaviorTree;

public:
	AEnemyAIController();

	void Aggro();

protected:
	virtual void BeginPlay() override;
};
