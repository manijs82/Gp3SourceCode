#include "SimpleGun.h"

#include "Gp3/Gp3Character.h"
#include "Gp3/CorePlayer/WeaponHandlerActorComponent.h"
#include "Kismet/GameplayStatics.h"

ASimpleGun::ASimpleGun()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASimpleGun::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmoCount = MaxAmmo;
	CurrentAmmoCountInMag = MaxAmmoPerMag;
	if(bInfiniteAmmo)
		CurrentAmmoCount = 100000;
}

void ASimpleGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASimpleGun::StartUse()
{
	Super::StartUse();

	TryShoot();
}

void ASimpleGun::EndUse()
{
	Super::EndUse();
	
	bCanCancelReload = true;
	bReloadCanceled = false;
}

void ASimpleGun::TryShoot()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if(TimerManager.IsTimerActive(ShootTimerHandle)) return;
	if(IsReloading())
	{
		if(bCancelableReload && bCanCancelReload)
		{
			CancelReload();
			bCanCancelReload = false;
			bReloadCanceled = true;
		}
		return;
	}

	if(bReloadCanceled  && bCanCancelReload) return;
	
	bCanCancelReload = false;
	Shoot();
	TimerManager.SetTimer(ShootTimerHandle, this, &ASimpleGun::EndCooldown, FireRate, true);
}

void ASimpleGun::Shoot()
{
	if(!HasAmmo(bAutoReloadWhenEmpty) && bNeedReload)
	{
		OnShotFailed();
		return;
	}

	if(bSpawnProjectiles)
	{
		SpawnProjectile();
	}
	CurrentAmmoCountInMag -= AmmoPerShot;

	OnShootGun();
	OnShoot.Broadcast(this);
	TObjectPtr<AGp3Character> owner = Cast<AGp3Character>(GetOwner());
	if(owner)
	{
		TObjectPtr<UWeaponHandlerActorComponent> WeaponHandler =
			Cast<UWeaponHandlerActorComponent>(owner->GetComponentByClass(UWeaponHandlerActorComponent::StaticClass()));
		WeaponHandler->OnShootGun.Broadcast();
	}
	
	bCanCancelReload = false;
	bReloadCanceled = true;
}

void ASimpleGun::SpawnProjectile()
{
	if(!GetOwner()) return;
	
	FVector ShootDirection = GetOwner()->GetActorForwardVector();
	ShootDirection.Y += FMath::RandRange(-ShootingInaccuracy, ShootingInaccuracy);
	ShootDirection.Normalize();

	
	
	FVector SpawnLoc = GetOwner()->GetActorLocation() + FVector(0.f, 0.f, ShootHeight) + GetOwner()->GetActorRightVector() * 10.f;
	FRotator Rotation = ShootDirection.Rotation();
	TObjectPtr<AProjectileBase> SpawnedProjectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, SpawnLoc, Rotation);

	SpawnedProjectile->StartProjectile(GetOwner(), MyShield);
}

void ASimpleGun::AddAmmo(int Count)
{
	CurrentAmmoCount += Count;
	CurrentAmmoCount = FMath::Min(CurrentAmmoCount, MaxAmmo);
}

void ASimpleGun::SetAmmoCount(int Count)
{
	CurrentAmmoCount = Count;
}

void ASimpleGun::SetAmmoCountInMag(int Count)
{
	CurrentAmmoCountInMag = Count;
}

bool ASimpleGun::CanAddAmmo()
{
	return CurrentAmmoCount < MaxAmmo;
}

bool ASimpleGun::HasAmmo(bool ReloadIfEmpty)
{
	if(CurrentAmmoCountInMag < AmmoPerShot)
	{
		if(ReloadIfEmpty)
			StartReload();
		return false;
	}

	return true;
}

void ASimpleGun::WeaponAction1()
{
	StartReload();

	Super::WeaponAction1();
}

void ASimpleGun::StartReload()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if(!IsReloading() && CurrentAmmoCountInMag != MaxAmmoPerMag && CurrentAmmoCount > 0)
	{
		TimerManager.SetTimer(ReloadTimerHandle, this, &ASimpleGun::Reload, ReloadTime, true);
		OnReloadStarted();
	}
}

void ASimpleGun::CancelReload()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if(IsReloading())
	{
		TimerManager.ClearTimer(ReloadTimerHandle);
		OnReloadCanceled();
	}
}

float ASimpleGun::GetReloadProgress()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if(IsReloading())
	{
		return TimerManager.GetTimerElapsed(ReloadTimerHandle) / ReloadTime;
	}
	
	return 0;
}

void ASimpleGun::Reload()
{
	if(CurrentAmmoCount <= 0) return;
	int tempAmmoCount = CurrentAmmoCount;
	CurrentAmmoCount -= FMath::Min(MaxAmmoPerMag - CurrentAmmoCountInMag, CurrentAmmoCount);	
	CurrentAmmoCountInMag += FMath::Min(MaxAmmoPerMag - CurrentAmmoCountInMag, tempAmmoCount);
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
	OnReloadCompleted();
}

bool ASimpleGun::IsReloading()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
 	return TimerManager.IsTimerActive(ReloadTimerHandle);
}

void ASimpleGun::EndCooldown()
{
	GetWorld()->GetTimerManager().ClearTimer(ShootTimerHandle);
}