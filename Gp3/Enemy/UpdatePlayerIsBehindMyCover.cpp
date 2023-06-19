// Fill out your copyright notice in the Description page of Project Settings.

#include "UpdatePlayerIsBehindMyCover.h"
#include "EnemyBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Gp3/Gp3Character.h"
#include "Kismet/GameplayStatics.h"

UUpdatePlayerIsBehindMyCover::UUpdatePlayerIsBehindMyCover()
{
	NodeName = "Update player is being my cover";
}

void UUpdatePlayerIsBehindMyCover::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	TObjectPtr<AEnemyBase> MyEnemy = Cast<AEnemyBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("SelfActor")));
	FVector PlayerLoc = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
	FVector EnemyLoc = MyEnemy->GetActorLocation();

	FHitResult Hit;
	FCollisionQueryParams TraceParams = FCollisionQueryParams(TEXT(""), false, MyEnemy); //Ignores self
	FVector TraceStart = EnemyLoc - FVector(0.f, 0.f, MyEnemy->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - 10.f);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, PlayerLoc, ECollisionChannel::ECC_Pawn, TraceParams);

	bool IsInCoverOfPlayer = Cast<AGp3Character>(Hit.GetActor()) == nullptr; //Hit actor is not the player, meaning enemy is still in cover of player

	if(!IsInCoverOfPlayer)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsInCoverOfPlayer"), false); //Don't allow this value to go back to true after becoming false once
	}
}

void UUpdatePlayerIsBehindMyCover::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}

void UUpdatePlayerIsBehindMyCover::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskDeactivated(Task);
}
