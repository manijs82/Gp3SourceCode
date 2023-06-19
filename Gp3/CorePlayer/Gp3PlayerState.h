// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Gp3/Interactions/DamageOverTimeField.h"
#include "Gp3/Interactions/Interactable.h"
#include "Gp3/Interactions/PackField.h"
#include "Gp3/Weapons/Weapon.h"
#include "Gp3PlayerState.generated.h"

UCLASS()
class GP3_API AGp3PlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AGp3PlayerState();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY()
	float Health;
	UPROPERTY()
	TArray<int> UnlockedWeapons;
	UPROPERTY()
	int CurrentWeapon;
	UPROPERTY()
	TArray<int> AmmoCounts;
	UPROPERTY()
	TArray<int> AmmoInMagCounts;
	UPROPERTY()
	int BombCount;
	UPROPERTY()
	int HealthPackCount;
	UPROPERTY()
	FVector PlayerPosition;
	UPROPERTY()
	float LookingAngle;
	UPROPERTY(BlueprintReadWrite)
	TArray<APackField*> DeadPacks;
	UPROPERTY(BlueprintReadWrite)
	TArray<ADamageOverTimeField*> DisabledMists;
	UPROPERTY(BlueprintReadWrite)
	TArray<AInteractable*> PickedUpInteractables;
};
