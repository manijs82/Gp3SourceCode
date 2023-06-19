#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

class UCharacterInteraction;

UCLASS()
class GP3_API AInteractable : public AActor
{
	GENERATED_BODY()

public:
	AInteractable();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void Interact(TObjectPtr<UCharacterInteraction> CharacterInteraction);
	UFUNCTION(BlueprintImplementableEvent)
	void OnInteract(UCharacterInteraction* CharacterInteraction);

	UFUNCTION(BlueprintImplementableEvent)
	void ShowUIPrompt();
	UFUNCTION(BlueprintImplementableEvent)
	void HideUIPrompt();
};
