// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Gp3/Interactions/DamageOverTimeField.h"
#include "Gp3/Interactions/Interactable.h"
#include "Gp3/Interactions/PackField.h"
#include "Gp3/Weapons/Weapon.h"
#include "Gp3SaveData.generated.h"

/**
 * 
 */
UCLASS()
class GP3_API UGp3SaveData : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	float Score;
	UPROPERTY(BlueprintReadOnly)
	float Health;
	UPROPERTY(BlueprintReadOnly)
	TArray<int> UnlockedWeapons;
	UPROPERTY(BlueprintReadOnly)
	int CurrentWeapon;
	UPROPERTY(BlueprintReadOnly)
	TArray<int> AmmoCounts;
	UPROPERTY(BlueprintReadOnly)
	TArray<int> AmmoInMagCounts;
	UPROPERTY(BlueprintReadOnly)
	int BombCount;
	UPROPERTY(BlueprintReadOnly)
	int HealthPackCount;
	UPROPERTY(BlueprintReadOnly)
	FVector PlayerPosition;
	UPROPERTY(BlueprintReadOnly)
	float LookingAngle;
	UPROPERTY(BlueprintReadOnly)
	TArray<APackField*> DeadPacks;
	UPROPERTY(BlueprintReadOnly)
	TArray<ADamageOverTimeField*> DisabledMists;
	UPROPERTY(BlueprintReadOnly)
	TArray<AInteractable*> PickedUpInteractables;
};
