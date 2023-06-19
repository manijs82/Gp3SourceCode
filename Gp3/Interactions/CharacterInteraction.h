#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Components/ActorComponent.h"
#include "CharacterInteraction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPickItem, AInteractable*, Item);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GP3_API UCharacterInteraction : public UActorComponent
{
	GENERATED_BODY()

	FTimerHandle DelayHandle;

public:
	UCharacterInteraction();

	TObjectPtr<UShapeComponent> OverlapArea;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void BindDelegates();

	UFUNCTION()
	void ShowUIPrompt(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void HideUIPrompt(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	virtual bool CheckInteraction();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bAutoPickup;
	UPROPERTY(BlueprintAssignable)
	FPickItem OnPickItem;
};
