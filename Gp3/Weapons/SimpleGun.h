#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Gp3/Projectiles/ProjectileBase.h"
#include "Sound/SoundCue.h"
#include "SimpleGun.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShootEvent, ASimpleGun*, Gun);

UCLASS()
class GP3_API ASimpleGun : public AWeapon
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Variables)
	bool bSpawnProjectiles = true;
	
	UPROPERTY(EditAnywhere, Category = Variables)
	bool bCancelableReload;

	UPROPERTY(EditAnywhere, Category = Variables)
	bool bInfiniteAmmo;

	UPROPERTY(EditAnywhere, Category = Variables)
	bool bAutoReloadWhenEmpty = true;

	UPROPERTY(EditAnywhere, Category = Variables)
	bool bNeedReload = true;
	
	UPROPERTY(EditAnywhere, Category = Variables)
	float ShootHeight = 50.f;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectileBase> ProjectileClass;

	bool bCanCancelReload = true;
	bool bReloadCanceled = false;

public:
	ASimpleGun();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Variables)
	float ShootingInaccuracy;

	UPROPERTY(EditAnywhere, Category = Variables)
	float FireRate = 0.2f;

	UPROPERTY(EditAnywhere, Category = Variables, BlueprintReadOnly)
	int MaxAmmo = 60;

	UPROPERTY(EditAnywhere, Category = Variables, BlueprintReadOnly)
	int MaxAmmoPerMag = 6;

	UPROPERTY(EditAnywhere, Category = Variables, BlueprintReadOnly)
	float ReloadTime = 3;

	UPROPERTY(EditAnywhere, Category = Variables)
	float AmmoPerShot = 1;
	
	FTimerHandle ShootTimerHandle;
	FTimerHandle ReloadTimerHandle;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void StartUse() override;
	virtual void EndUse() override;
	UFUNCTION(BlueprintCallable)
	void TryShoot();
	void Shoot();
	bool HasAmmo(bool ReloadIfEmpty = false);
	virtual void WeaponAction1() override;
	UFUNCTION(BlueprintCallable)
	virtual void StartReload();
	UFUNCTION(BlueprintCallable)
	virtual void CancelReload();
	UFUNCTION(BlueprintCallable)
	float GetReloadProgress();
	virtual void Reload();
	void EndCooldown();
	UFUNCTION(BlueprintCallable)
	virtual void SpawnProjectile();
	UFUNCTION(BlueprintCallable)
	void AddAmmo(int Count);
	UFUNCTION(BlueprintCallable)
	void SetAmmoCount(int Count);
	UFUNCTION(BlueprintCallable)
	void SetAmmoCountInMag(int Count);
	UFUNCTION(BlueprintCallable)
	bool CanAddAmmo();
	UFUNCTION(BlueprintCallable)
	bool IsReloading();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnShootGun();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnReloadStarted();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnReloadCanceled();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnReloadCompleted();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnShotFailed();

	UPROPERTY(BlueprintReadOnly)
	int CurrentAmmoCountInMag;
	UPROPERTY(BlueprintReadOnly)
	int CurrentAmmoCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundCue> ShootSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundCue> EmptyMagShoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundCue> ReloadSound;
	UPROPERTY(BlueprintAssignable)
	FShootEvent OnShoot;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> MyShield;
};
