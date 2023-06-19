// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

class UHealthActorComponent;
class ACoverPoint;
class ASimpleGun;
class AEnemyAIController;

UCLASS()
class GP3_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAlerted);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHit);

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UHealthActorComponent> HealthActorComponent;

public:
	AEnemyBase();

	UPROPERTY(EditAnywhere)
	TObjectPtr<ACoverPoint> MyCoverPoint;

	UPROPERTY(EditAnywhere)
	float EnemyFOV;

	UPROPERTY(EditAnywhere)
	float EnemySightRange;

	UPROPERTY(EditAnywhere, meta = (UIMin = "0.01", UIMax = "0.5"))
	float EnemyTurningSpeed = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanDropItems = true;
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<ASimpleGun> MyWeapon;
	
	TObjectPtr<AActor> GetCoverPoint();
	bool IsInCover();

	FOnAlerted OnAlerted;
	FOnDeath OnDeath;
	FOnHit OnHit;

	void Alert();

	void EnemyCrouch();
	void EnemyUnCrouch();

	UPROPERTY(EditAnywhere)
	float UncrouchedHeight = 96.f;
	UPROPERTY(EditAnywhere)
	float CrouchHeight = 32.f;

	UPROPERTY(BlueprintReadOnly)
	bool bIsEnemyCrouched = false;

	//Sound effects
	FTimerHandle SfxCooldownHandle;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundCue> DeathSoundCue;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundCue> DamagedSoundCue;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundCue> IdleSoundCue;
	
	UPROPERTY(EditAnywhere)
	float SfxCooldownTime = 2.f;
	UFUNCTION(BlueprintCallable)
	void PlaySfx(USoundCue* SfxCue);
	void SfxCooldown();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void Die();
	UFUNCTION()
	void Hit();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ASimpleGun> MyWeaponTemplate;

	TObjectPtr<AEnemyAIController> MyController;
};
