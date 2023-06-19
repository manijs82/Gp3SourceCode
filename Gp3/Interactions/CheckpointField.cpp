// Fill out your copyright notice in the Description page of Project Settings.

#include "CheckpointField.h"

#include "DamageOverTimeField.h"
#include "PackField.h"
#include "Gp3/Gp3Character.h"
#include "Gp3/CorePlayer/Gp3PlayerState.h"
#include "Gp3/CorePlayer/HealthActorComponent.h"
#include "Gp3/CorePlayer/WeaponHandlerActorComponent.h"
#include "Gp3/Managers/Gp3GameState.h"
#include "Gp3/Weapons/SimpleGun.h"
#include "Kismet/GameplayStatics.h"

ACheckpointField::ACheckpointField()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACheckpointField::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(CanUseCheckpointHandle, this, &ACheckpointField::CanUseCheckpointCooldown, 3.f); //Initial cooldown
}

void ACheckpointField::CanUseCheckpointCooldown()
{
	GetWorld()->GetTimerManager().ClearTimer(CanUseCheckpointHandle);
}

void ACheckpointField::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if(HasSaved) return;
	if(GetWorld()->GetTimerManager().IsTimerActive(CanUseCheckpointHandle)) return; //Initial cooldown
	
	TObjectPtr<AGp3Character> PlayerCharacter = Cast<AGp3Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	if(!PlayerCharacter) return;
	if(OtherActor != PlayerCharacter) return; //Didn't overlap with player

	//Get values to save
	TObjectPtr<AGp3PlayerState> PlayerState = Cast<AGp3PlayerState>(PlayerCharacter->GetPlayerState());
	TObjectPtr<UWeaponHandlerActorComponent> WeaponHandler =
		Cast<UWeaponHandlerActorComponent>(PlayerCharacter->GetComponentByClass(UWeaponHandlerActorComponent::StaticClass()));
	PlayerState->PlayerPosition = PlayerCharacter->GetActorLocation();
	PlayerState->Health = PlayerCharacter->HealthActorComponent->GetCurrentHealth();
	PlayerState->AmmoCounts.Empty();
	PlayerState->AmmoInMagCounts.Empty();
	for (auto Weapon : WeaponHandler->UnlockedWeapons)
	{
		TObjectPtr<ASimpleGun> gun = Cast<ASimpleGun>(Weapon);
		PlayerState->AmmoCounts.Add(gun->CurrentAmmoCount);
		PlayerState->AmmoInMagCounts.Add(gun->CurrentAmmoCountInMag);
	}
	PlayerState->BombCount = WeaponHandler->BombCount;
	PlayerState->HealthPackCount = PlayerCharacter->HealthpackAmount;
	PlayerState->CurrentWeapon = WeaponHandler->GetWeaponIndex(WeaponHandler->CurrentWeapon);
	PlayerState->LookingAngle = PlayerCharacter->CurrentLookingAngle;

	PlayerState->DeadPacks.Empty();
	TArray<AActor*> packs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APackField::StaticClass(), packs);
	for (AActor* pack : packs)
	{
		TObjectPtr<APackField> p = Cast<APackField>(pack);
		if(p->bAreAllDead)
		{
			PlayerState->DeadPacks.Add(p);
		}
	}

	PlayerState->DisabledMists.Empty();
	TArray<AActor*> mists;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADamageOverTimeField::StaticClass(), mists);
	for (AActor* mist : mists)
	{
		TObjectPtr<ADamageOverTimeField> m = Cast<ADamageOverTimeField>(mist);
		if(m->bIsDisabled)
		{
			PlayerState->DisabledMists.Add(m);
		}
	}

	PlayerState->UnlockedWeapons.Empty();
	for (TObjectPtr<AWeapon> Weapon : WeaponHandler->UnlockedWeapons)
	{
		PlayerState->UnlockedWeapons.Add(WeaponHandler->GetWeaponIndex(Weapon));	
	}	

	//Save game
	TObjectPtr<AGp3GameState> GameState = Cast<AGp3GameState>(UGameplayStatics::GetGameState(GetWorld()));
	GameState->SaveGameData();

	OnSave.Broadcast();
	
	HasSaved = true;
}

