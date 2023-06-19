#include "PackField.h"

APackField::APackField()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APackField::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(InitTimerHandle, this, &APackField::InitEnemies, 0.2f);
}

void APackField::InitEnemies()
{
	if(bIsKilled) return;
	TArray<AActor*> Actors;
	BoxCollider->GetOverlappingActors(Actors, AEnemyBase::StaticClass());

	for (AActor* Actor : Actors)
	{
		AEnemyBase* Enemy = Cast<AEnemyBase>(Actor);
		Enemies.Add(Enemy);

		Enemy->OnAlerted.AddDynamic(this, &APackField::OnFirstEnemyAlerted);
		Enemy->OnDeath.AddDynamic(this, &APackField::OnEnemyDied);
		Enemy->OnHit.AddDynamic(this, &APackField::AlertAllInPack);
	}

	EnemyCount = Enemies.Num();
}

void APackField::OnFirstEnemyAlerted()
{
	if(bIsKilled) return;
	if(bFirstEnemyAlerted) return;
	bFirstEnemyAlerted = true;

	AlertAllInPack();
	OnFirstMemberAlerted();
}

void APackField::OnEnemyDied()
{
	if(bIsKilled) return;
	DeadEnemyCount++;

	if(DeadEnemyCount == 1) // alert all when first enemy dies
	{
		AlertAllInPack();
	}

	OnPackMemberDied();
	if(DeadEnemyCount >= EnemyCount)
	{
		bAreAllDead = true;
		OnAllDead();
	}
}

void APackField::OnAllDead()
{
	if(bIsKilled) return;
	OnPackDied();
	OnAllMembersDead.Broadcast();
}

void APackField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APackField::AlertAllInPack()
{
	if(bIsKilled) return;
	if(bHasAlertedAll) return;
	bHasAlertedAll = true;
	for (TObjectPtr<AEnemyBase> Enemy : Enemies)
	{
		if(!Enemy) continue;
		Enemy->Alert();
	}
	OnAlertAllEvent.Broadcast();
}

void APackField::KillAll()
{
	TArray<AActor*> Actors;
	BoxCollider->GetOverlappingActors(Actors, AEnemyBase::StaticClass());

	for (AActor* Actor : Actors)
	{
		Actor->Destroy();
	}

	bAreAllDead = true;
	bIsKilled = true;
}

