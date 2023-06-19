#pragma once

#include "CoreMinimal.h"
#include "TriggerField.h"
#include "Gp3/Gp3Character.h"
#include "Gp3/Enemy/EnemyBase.h"
#include "PackField.generated.h"

UCLASS()
class GP3_API APackField : public ATriggerField
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAlertAllEvent);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAllMembersDead);

	FTimerHandle InitTimerHandle;

public:
	APackField();

protected:
	virtual void BeginPlay() override;
	void InitEnemies();
	UFUNCTION()
	void OnFirstEnemyAlerted();
	UFUNCTION()
	void OnEnemyDied();
	void OnAllDead();

	bool bFirstEnemyAlerted;
	int DeadEnemyCount;

public:
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnPackMemberDied();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnPackDied();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnFirstMemberAlerted();
	UFUNCTION(BlueprintCallable)
	void AlertAllInPack();
	UFUNCTION(BlueprintCallable)
	void KillAll();
	
	UPROPERTY(BlueprintAssignable)
	FAlertAllEvent OnAlertAllEvent;
	UPROPERTY(BlueprintAssignable)
	FAllMembersDead OnAllMembersDead;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<AEnemyBase>> Enemies;
	UPROPERTY(BlueprintReadOnly)
	bool bHasAlertedAll;
	UPROPERTY()
	TObjectPtr<AGp3Character> PlayerCharacter;
	UPROPERTY(BlueprintReadOnly)
	int EnemyCount;
	bool bAreAllDead;
	bool bIsKilled;
};
