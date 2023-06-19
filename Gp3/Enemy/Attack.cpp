// Fill out your copyright notice in the Description page of Project Settings.

#include "Attack.h"
#include "EnemyAIController.h"
#include "EnemyBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gp3/Weapons/SimpleGun.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Chaos/ChaosDebugDraw.h"

UAttack::UAttack()
{
	NodeName = "Attack";
}

EBTNodeResult::Type UAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	TObjectPtr<AEnemyBase> MyEnemy = Cast<AEnemyBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("SelfActor")));
	TObjectPtr<AActor> PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	MyEnemy->GetCharacterMovement()->bOrientRotationToMovement = false;

	//Rotate towards player
	FRotator CurrentRot = MyEnemy->GetActorRotation();
	FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(MyEnemy->GetActorLocation(), PlayerActor->GetActorLocation());
	
	FRotator NewRot = UKismetMathLibrary::RLerp(CurrentRot, TargetRot, MyEnemy->EnemyTurningSpeed, true);
	MyEnemy->SetActorRotation(NewRot);

	//Don't shoot if another enemy is in front of me
	FHitResult Hit;
	FHitResult Hit2;
	FCollisionQueryParams TraceParams = FCollisionQueryParams(TEXT(""), false, MyEnemy); //Ignores self
	FVector TraceEnd = MyEnemy->GetActorLocation() + MyEnemy->GetActorForwardVector() * 10000.f;
	FCollisionShape Shape = FCollisionShape::MakeBox(FVector(65.f, 65.f, 65.f));
	GetWorld()->SweepSingleByObjectType(Hit, MyEnemy->GetActorLocation(), TraceEnd, MyEnemy->GetActorRotation().Quaternion(), ECollisionChannel::ECC_Pawn, Shape, TraceParams);
	GetWorld()->LineTraceSingleByChannel(Hit2, MyEnemy->GetActorLocation(), TraceEnd, ECollisionChannel::ECC_Pawn, TraceParams);

	if(Cast<AEnemyBase>(Hit.GetActor()) || Cast<AEnemyBase>(Hit2.GetActor())) return EBTNodeResult::Succeeded; //Theres an enemy in front of me
		

	//Wait a bit before starting to attack for first attack
	if(bFirstTimeShooting && !GetWorld()->GetTimerManager().IsTimerActive(FirstShotTimerHandle))
	{
		float WaitTime = FMath::RandRange(1.f, 2.f);
		GetWorld()->GetTimerManager().SetTimer(FirstShotTimerHandle, this, &UAttack::FirstTimeShootingCooldown, WaitTime);
	}

	//Attack
	if(!GetWorld()->GetTimerManager().IsTimerActive(FirstShotTimerHandle))
	{
		MyEnemy->MyWeapon->StartUse();
	}
	
	return EBTNodeResult::Succeeded;
}

void UAttack::FirstTimeShootingCooldown()
{
	bFirstTimeShooting = false;
}

void UAttack::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}

void UAttack::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskDeactivated(Task);
}

