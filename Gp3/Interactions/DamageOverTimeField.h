#pragma once

#include "CoreMinimal.h"
#include "TriggerField.h"
#include "Gp3/CorePlayer/HealthActorComponent.h"
#include "DamageOverTimeField.generated.h"

UCLASS()
class GP3_API ADamageOverTimeField : public ATriggerField
{
	GENERATED_BODY()
	
	FTimerHandle DamageTimerHandle;
	
public:
	ADamageOverTimeField();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	virtual void DamageHealths();
	UFUNCTION(BlueprintCallable)
	void DisableDamaging();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnDamage();
	UFUNCTION(BlueprintImplementableEvent)
	void OnDisabled();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int DamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageRate = 2.5f;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<UHealthActorComponent>> Healths;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanDamageEnemies = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanDisable;

	UPROPERTY(BlueprintReadOnly)
	bool bIsDisabled;
	
};
