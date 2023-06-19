// Fill out your copyright notice in the Description page of Project Settings.

#include "SetIfSeesPlayer.h"
#include "EnemyAIController.h"
#include "EnemyBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Gp3/Gp3Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

USetIfSeesPlayer::USetIfSeesPlayer()
{
	NodeName = "Set vision values";
}

void USetIfSeesPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	TObjectPtr<AEnemyBase> MyEnemy = Cast<AEnemyBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("SelfActor")));

	FVector PlayerLoc = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
	FVector EnemyLoc = MyEnemy->GetActorLocation();

	FVector EnemyForwardVector = MyEnemy->GetActorForwardVector();
	FVector EnemyToPlayerVector = PlayerLoc - EnemyLoc;

	float EnemyToPlayerAngle = UKismetMathLibrary::DegAcos(UKismetMathLibrary::Dot_VectorVector(EnemyForwardVector, EnemyToPlayerVector) / (EnemyForwardVector.Length() * EnemyToPlayerVector.Length()));

	bool IsWithinSightRange = EnemyToPlayerVector.Length() <= MyEnemy->EnemySightRange;
	bool IsWithinFOV = EnemyToPlayerAngle < MyEnemy->EnemyFOV;

	//Check if player aggro's the enemy
	bool SeesPlayer = false;
	if(IsWithinSightRange && IsWithinFOV)
	{
		FHitResult Hit;
		FCollisionQueryParams TraceParams = FCollisionQueryParams(TEXT(""), false, MyEnemy); //Ignores self
		GetWorld()->LineTraceSingleByChannel(Hit, EnemyLoc, PlayerLoc, ECollisionChannel::ECC_Pawn, TraceParams);

		SeesPlayer = Cast<AGp3Character>(Hit.GetActor()) != nullptr; //Has direct sight of player
	}
	
	if(SeesPlayer)
	{
		MyEnemy->Alert();
	}
}

void USetIfSeesPlayer::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}

void USetIfSeesPlayer::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskDeactivated(Task);
}
