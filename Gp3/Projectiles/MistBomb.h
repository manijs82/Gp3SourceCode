#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MistBomb.generated.h"

UCLASS()
class GP3_API AMistBomb : public AActor
{
	GENERATED_BODY()

public:
	AMistBomb();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Launch(float Force);

	UFUNCTION(BlueprintImplementableEvent)
	void OnLaunch(float Force);
};
