// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class GP3_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool EndUseOnMouseUp = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsUnlocked;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMesh> WeaponStaticMesh;

	UPROPERTY(BlueprintReadOnly)
	bool bIsSelected;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void StartUse();
	virtual void TriggerUse();
	virtual void EndUse();
	virtual void WeaponAction1();
	virtual void WeaponAction2();
	virtual void WeaponAction3();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnStartUsingWeapon();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnTriggerUseWeapon();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnEndUseWeapon();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnUsedWeaponAction1();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnUsedWeaponAction2();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnUsedWeaponAction3();
	UFUNCTION(BlueprintCallable)
	virtual void Select(bool active) { bIsSelected = active; }
};
