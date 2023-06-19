#include "Shotgun.h"

AShotgun::AShotgun()
{
	PrimaryActorTick.bCanEverTick = true;
	ShootingInaccuracy = 0.2f;
	AmmoPerShot = 2;
}

void AShotgun::BeginPlay()
{
	Super::BeginPlay();
}

void AShotgun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShotgun::SpawnProjectile()
{
	for (int i = 0; i < ProjectilePerShot; i++)
	{
		Super::SpawnProjectile();
	}
}

void AShotgun::Reload()
{
	if(CurrentAmmoCount <= 0) return;
	CurrentAmmoCount -= 1;	
	CurrentAmmoCountInMag += 1;
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);

	if(CurrentAmmoCountInMag < MaxAmmoPerMag)
	{
		StartReload();
	}
	else
	{
		OnReloadCompleted();		
	}
}

