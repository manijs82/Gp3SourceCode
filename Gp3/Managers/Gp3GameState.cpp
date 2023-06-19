#include "Gp3GameState.h"
#include "Gp3/CorePlayer/Gp3PlayerState.h"
#include "Gp3/Saving/Gp3SaveData.h"
#include "Kismet/GameplayStatics.h"

AGp3GameState::AGp3GameState()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGp3GameState::BeginPlay()
{
	Super::BeginPlay();
	GameData = Cast<UGp3SaveData>(UGameplayStatics::CreateSaveGameObject(UGp3SaveData::StaticClass()));	
}

void AGp3GameState::SaveGameData() const
{
	if(PlayerArray.IsEmpty()) return;
	TObjectPtr<AGp3PlayerState> PlayerState = Cast<AGp3PlayerState>(PlayerArray[0]);
	
	GameData->Score = PlayerState->GetScore();
	GameData->PlayerPosition = PlayerState->PlayerPosition;
	GameData->Health = PlayerState->Health;
	GameData->UnlockedWeapons = PlayerState->UnlockedWeapons;
	GameData->AmmoCounts = PlayerState->AmmoCounts;
	GameData->AmmoInMagCounts = PlayerState->AmmoInMagCounts;
	GameData->BombCount = PlayerState->BombCount;
	GameData->HealthPackCount = PlayerState->HealthPackCount;
	GameData->CurrentWeapon = PlayerState->CurrentWeapon;
	GameData->LookingAngle = PlayerState->LookingAngle;
	GameData->DeadPacks = PlayerState->DeadPacks;
	GameData->DisabledMists = PlayerState->DisabledMists;
	GameData->PickedUpInteractables = PlayerState->PickedUpInteractables;

	if(!GameData) return;
	UGameplayStatics::SaveGameToSlot(GameData, SaveFileName, 0);
}

UGp3SaveData* AGp3GameState::GetGameData() const
{
	return Cast<UGp3SaveData>(UGameplayStatics::LoadGameFromSlot(SaveFileName, 0));
}

void AGp3GameState::DeleteSaveGameData()
{
	UGameplayStatics::DeleteGameInSlot(SaveFileName, 0);
}


