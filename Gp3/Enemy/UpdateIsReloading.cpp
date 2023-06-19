// Fill out your copyright notice in the Description page of Project Settings.

#include "UpdateIsReloading.h"
#include "EnemyBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Gp3/Weapons/SimpleGun.h"

UUpdateIsReloading::UUpdateIsReloading()
{
	NodeName = "Update is reloading";
}

void UUpdateIsReloading::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	TObjectPtr<AEnemyBase> MyEnemy = Cast<AEnemyBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("SelfActor")));
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsReloading"), MyEnemy->MyWeapon->IsReloading());
}

void UUpdateIsReloading::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}

void UUpdateIsReloading::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskDeactivated(Task);
}
