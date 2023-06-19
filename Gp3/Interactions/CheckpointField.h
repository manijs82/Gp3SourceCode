#pragma once

#include "CoreMinimal.h"
#include "TriggerField.h"
#include "CheckpointField.generated.h"

class AGp3Character;

UCLASS()
class GP3_API ACheckpointField : public ATriggerField
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSaveDelegate);

	FTimerHandle CanUseCheckpointHandle;
	void CanUseCheckpointCooldown();
	
public:
	ACheckpointField();

	virtual void BeginPlay() override;

protected:
	bool HasSaved = false;

public:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(BlueprintAssignable)
	FSaveDelegate OnSave;
};
