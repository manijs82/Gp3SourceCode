// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Gp3/Gp3Character.h"
#include "Gp3/Projectiles/MistBomb.h"
#include "Gp3/Weapons/Weapon.h"
#include "WeaponHandlerActorComponent.generated.h"

class AProjectileBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEquipWeapon, AWeapon*, NewWeapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FThrowBomb, AMistBomb*, Bomb);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartAim);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTriggerAim);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FThrowFailed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShootGun);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GP3_API UWeaponHandlerActorComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Variables)
	TArray<TSubclassOf<AWeapon>> WeaponClasses;
	
	UPROPERTY(EditAnywhere, Category = Variables)
	TSubclassOf<AWeapon> DefaultWeapon;

	UPROPERTY(EditAnywhere, Category = Variables)
	TSubclassOf<AMistBomb> BombClass;

	UPROPERTY(EditAnywhere, Category = Variables)
	int BombLimit = 3;

public:
	UWeaponHandlerActorComponent();

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<AWeapon>> Weapons;

	UFUNCTION(BlueprintCallable)
	void EquipWeapon(TSubclassOf<AWeapon> weaponClass);
	UFUNCTION(BlueprintCallable)
	void EquipWeaponByReference(AWeapon* weapon);
	UFUNCTION(BlueprintCallable)
	void EquipWeaponByIndex(int index);
	UFUNCTION(BlueprintCallable)
	void UnlockAndEquipWeapon(TSubclassOf<AWeapon> weaponClass);
	UFUNCTION(BlueprintCallable)
	void UnlockAndEquipWeaponByReference(AWeapon* weapon);
	UFUNCTION(BlueprintCallable)
	AWeapon* UnlockAndEquipWeaponByIndex(int index);
	void SelectWeapon(TObjectPtr<AWeapon> weapon);
	void SelectWeapon(int index);
	void SelectUnlockedWeapon(int index);
	void ScrollWeapon(int delta);
	void StartUse();
	void TriggerUse();
	void EndUse();
	void CallWeaponAction(int index);
	int GetWeaponIndex(TObjectPtr<AWeapon> Weapon);

	void StartThrow();
	void AimThrow(float DeltaTime);
	void ThrowBomb();
	UFUNCTION(BlueprintCallable)
	void AddBomb();
	UFUNCTION(BlueprintCallable)
	bool CanPickBombs();
	UFUNCTION(BlueprintCallable)
	void SetBomb(int Count);

	UPROPERTY(BlueprintAssignable)
	FTriggerAim OnAim;
	UPROPERTY(BlueprintAssignable)
	FStartAim OnStartAim;
	UPROPERTY(BlueprintAssignable)
	FThrowBomb OnThrow;
	UPROPERTY(BlueprintAssignable)
	FThrowFailed OnThrowFailed;
	UPROPERTY(BlueprintAssignable)
	FShootGun OnShootGun;
	
	UPROPERTY(BlueprintAssignable)
	FEquipWeapon OnEquipWeapon;

	TObjectPtr<AGp3Character> MyPlayer;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AWeapon> CurrentWeapon;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AWeapon> PreviousWeapon;

	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<AWeapon>> UnlockedWeapons;

	UPROPERTY(BlueprintReadOnly)
	int BombCount;
	UPROPERTY(EditAnywhere)
	float ForceIncreaseSpeed = 400;
	UPROPERTY(EditAnywhere)
	float MaxThrowForce = 750;
	UPROPERTY(BlueprintReadOnly)
	float ThrowForce;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ForceAcceptanceMultiplier = 2;

	UPROPERTY(EditAnywhere, Category = Variables, BlueprintReadOnly)
	float BombThrowOffset = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Variables)
	TObjectPtr<UStaticMeshComponent> GunStaticMeshComponent;

protected:
	virtual void BeginPlay() override;
	bool HasAccessToWeapon(TObjectPtr<AWeapon> weapon);
};
