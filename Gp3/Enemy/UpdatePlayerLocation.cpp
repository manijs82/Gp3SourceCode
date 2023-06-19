// Fill out your copyright notice in the Description page of Project Settings.

#include "UpdatePlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UUpdatePlayerLocation::UUpdatePlayerLocation()
{
	NodeName = "Update player location";
}

void UUpdatePlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("PlayerLoc"), UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void UUpdatePlayerLocation::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}

void UUpdatePlayerLocation::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskDeactivated(Task);
}
