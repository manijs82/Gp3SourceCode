#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class GP3_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	AEnemySpawner();

	UPROPERTY(EditAnywhere, Category = Variables)
	TArray<TSubclassOf<AEnemyBase>> EnemyClasses;

	UPROPERTY(EditAnywhere, Category = Variables)
	FVector2D SpawnRange;

	UPROPERTY(EditAnywhere, Category = Variables)
	float MinSpawnRate = 4;
	UPROPERTY(EditAnywhere, Category = Variables)
	float MaxSpawnRate = 6;
	
	FTimerHandle SpawnTimerHandle;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();
	UFUNCTION(BlueprintCallable)
	void StartSpawning();
	
	FVector GetRandomPoint();
};
