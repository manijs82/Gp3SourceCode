#include "Weapon.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::StartUse()
{
	OnStartUsingWeapon();
}

void AWeapon::TriggerUse()
{
	OnTriggerUseWeapon();
}

void AWeapon::EndUse()
{
	if(EndUseOnMouseUp)
	{
		OnEndUseWeapon();
	}
}

void AWeapon::WeaponAction1()
{
	OnUsedWeaponAction1();
}

void AWeapon::WeaponAction2()
{
	OnUsedWeaponAction2();
}

void AWeapon::WeaponAction3()
{
	OnUsedWeaponAction3();
}

