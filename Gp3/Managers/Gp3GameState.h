#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Gp3GameState.generated.h"

class UGp3SaveData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLoadEvent, UGp3SaveData*, GameData);

UCLASS()
class GP3_API AGp3GameState : public AGameStateBase
{
	GENERATED_BODY()

	const FString SaveFileName = FString("Save");

public:
	AGp3GameState();
	
	UPROPERTY(BlueprintAssignable)
	FLoadEvent OnLoadGameData;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UGp3SaveData> GameData;
	
public:
	UFUNCTION(BlueprintCallable)
	UGp3SaveData* GetGameData() const;
	UFUNCTION(BlueprintCallable)
	void SaveGameData() const;
	UFUNCTION(BlueprintCallable)
	void DeleteSaveGameData();
};
