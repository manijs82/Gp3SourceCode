#pragma once

#include "CoreMinimal.h"
#include "SimpleGun.h"
#include "Shotgun.generated.h"

UCLASS()
class GP3_API AShotgun : public ASimpleGun
{
	GENERATED_BODY()

public:
	AShotgun();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int ProjectilePerShot = 1;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SpawnProjectile() override;
	virtual void Reload() override;
};
