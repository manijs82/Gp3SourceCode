// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthActorComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GP3_API UHealthActorComponent : public UActorComponent
{
	GENERATED_BODY()

	FTimerHandle NaturalHealHandle;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateHealthbarDelegate, float, Health);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

	UPROPERTY(EditAnywhere, Category = Variables)
	bool bShouldNaturallyHeal = false;

	UPROPERTY(EditAnywhere, Category = Variables)
	float TimeUntilNaturalHealStarts = 5.f;

	UPROPERTY(EditAnywhere, Category = Variables)
	float NaturalHealPower = 10.f;

	UPROPERTY(EditAnywhere, Category = Variables, meta = (UIMin = "0.0", UIMax = "1.0"))
	float NaturalHealLimit = 1.f;

public:
	UHealthActorComponent();

	void UpdateHealthBy(float Hp);

	UPROPERTY(BlueprintAssignable)
	FOnUpdateHealthbarDelegate OnHealthUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;

	UPROPERTY(BlueprintAssignable)
	FOnDeath OnHit;

	UPROPERTY(BlueprintReadWrite)
	bool bIsDead = false;

	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth() const {return CurrentHealth;}
	UFUNCTION(BlueprintCallable)
	void SetCurrentHealth(float Hp) {CurrentHealth = Hp;}
	UFUNCTION(BlueprintCallable)
	float GetCurrentHealthPercentage() {return CurrentHealth / MaxHealth;}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsInvulnerable = false;

protected:
	virtual void BeginPlay() override;

	void NaturalHeal();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Variables)
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, Category = Variables)
	float CurrentHealth;
};
