#include "EnemySpawner.h"

#include "Kismet/GameplayStatics.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemySpawner::SpawnEnemy()
{
	TObjectPtr<APawn> PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	
	TSubclassOf<AEnemyBase> RandEnemyClass = EnemyClasses[FMath::RandRange(0, EnemyClasses.Num() - 1)];
	TObjectPtr<AEnemyBase> Enemy = GetWorld()->SpawnActor<AEnemyBase>(RandEnemyClass, GetRandomPoint(), FRotator::ZeroRotator);

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(SpawnTimerHandle, this, &AEnemySpawner::SpawnEnemy, FMath::RandRange(MinSpawnRate, MaxSpawnRate));
	if(!PlayerPawn) return;
	if(!Enemy) return;
	Enemy->SetActorRotation((PlayerPawn->GetActorLocation() - Enemy->GetActorLocation()).Rotation());
	Enemy->Alert();
}

void AEnemySpawner::StartSpawning()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();	
	TimerManager.SetTimer(SpawnTimerHandle, this, &AEnemySpawner::SpawnEnemy, FMath::RandRange(MinSpawnRate, MaxSpawnRate));
}

FVector AEnemySpawner::GetRandomPoint()
{
	float RandDistance = FMath::RandRange(SpawnRange.X, SpawnRange.Y);
	FVector Forward = GetActorForwardVector();
	FVector Direction = Forward.RotateAngleAxis(FMath::RandRange(-90, 90), GetActorUpVector());
	FVector Point = Direction * RandDistance;

	return GetActorLocation() + Point;
}

