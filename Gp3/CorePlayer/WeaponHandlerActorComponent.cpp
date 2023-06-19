#include "WeaponHandlerActorComponent.h"

#include "Kismet/GameplayStatics.h"

UWeaponHandlerActorComponent::UWeaponHandlerActorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponHandlerActorComponent::BeginPlay()
{
	Super::BeginPlay();

	MyPlayer = Cast<AGp3Character>(GetOwner());

	for (TSubclassOf<AWeapon> weaponClass : WeaponClasses)
	{
		TObjectPtr<AWeapon> weapon = GetWorld()->SpawnActor<AWeapon>(weaponClass, FVector::Zero(), FRotator::ZeroRotator);
		Weapons.Add(weapon);
		weapon->SetOwner(GetOwner());
	}
	
	if(DefaultWeapon)
	{
		UnlockAndEquipWeapon(DefaultWeapon);
	}
}

void UWeaponHandlerActorComponent::StartUse()
{
	if(!CurrentWeapon) return;
	if(HasAccessToWeapon(CurrentWeapon))
	{
		CurrentWeapon->StartUse();
	}	
}

void UWeaponHandlerActorComponent::TriggerUse()
{
	if(!CurrentWeapon) return;
	if(HasAccessToWeapon(CurrentWeapon))
	{
		CurrentWeapon->TriggerUse();
	}	
}

void UWeaponHandlerActorComponent::EndUse()
{
	if(!CurrentWeapon) return;
	if(HasAccessToWeapon(CurrentWeapon))
	{
		CurrentWeapon->EndUse();
	}	
}

void UWeaponHandlerActorComponent::CallWeaponAction(int index)
{
	switch(index)
	{
	case 1:
		CurrentWeapon->WeaponAction1();
		break;
	case 2:
		CurrentWeapon->WeaponAction2();
		break;
	case 3:
		CurrentWeapon->WeaponAction3();
		break;
	default: ;
	}
}

int UWeaponHandlerActorComponent::GetWeaponIndex(TObjectPtr<AWeapon> Weapon)
{
	for (int i = 0; i < Weapons.Num(); i++)
	{
		if(Weapons[i] == Weapon)
			return i;
	}
	return 0;
}

void UWeaponHandlerActorComponent::EquipWeapon(TSubclassOf<AWeapon> weaponClass)
{
	for (int i = 0; i < WeaponClasses.Num(); i++)
	{
		if(!Weapons[i]->IsA(weaponClass) || !Weapons[i]->IsUnlocked) continue; // cant equip if is locked or if is a different class
		
		SelectWeapon(i);
		OnEquipWeapon.Broadcast(CurrentWeapon);
		return;
	}
}

void UWeaponHandlerActorComponent::EquipWeaponByReference(AWeapon* weapon)
{
	if(!Weapons.Contains(weapon) || !weapon->IsUnlocked) return; // cant equip if is locked or is not included in the list

	SelectWeapon(weapon);
	OnEquipWeapon.Broadcast(CurrentWeapon);
}

void UWeaponHandlerActorComponent::EquipWeaponByIndex(int index)
{
	if(UnlockedWeapons.IsEmpty()) return;
	if(UnlockedWeapons.Num() <= index) return;
	EquipWeaponByReference(UnlockedWeapons[index]);
}

void UWeaponHandlerActorComponent::UnlockAndEquipWeapon(TSubclassOf<AWeapon> weaponClass)
{
	for (int i = 0; i < WeaponClasses.Num(); i++)
	{
		if(!Weapons[i]->IsA(weaponClass)) continue;

		SelectWeapon(i);
		CurrentWeapon->IsUnlocked = true;
		if(!UnlockedWeapons.Contains(CurrentWeapon))
		{
			UnlockedWeapons.Add(CurrentWeapon);
		}
		OnEquipWeapon.Broadcast(CurrentWeapon);
		return;
	}
}

void UWeaponHandlerActorComponent::UnlockAndEquipWeaponByReference(AWeapon* weapon)
{
	if(!Weapons.Contains(weapon)) return;

	SelectWeapon(weapon);
	CurrentWeapon->IsUnlocked = true;
	if(!UnlockedWeapons.Contains(CurrentWeapon))
	{
		UnlockedWeapons.Add(CurrentWeapon);
	}
	OnEquipWeapon.Broadcast(CurrentWeapon);
}

AWeapon* UWeaponHandlerActorComponent::UnlockAndEquipWeaponByIndex(int index)
{
	UnlockAndEquipWeaponByReference(Weapons[index]);
	return Weapons[index];
}

void UWeaponHandlerActorComponent::SelectWeapon(TObjectPtr<AWeapon> weapon)
{
	if(CurrentWeapon)
	{
		CurrentWeapon->Select(false);
		PreviousWeapon = CurrentWeapon;
	}
	CurrentWeapon = weapon;
	CurrentWeapon->Select(true);
	if(GunStaticMeshComponent && CurrentWeapon->WeaponStaticMesh)
	{
		GunStaticMeshComponent->SetStaticMesh(CurrentWeapon->WeaponStaticMesh);
	}
}

void UWeaponHandlerActorComponent::SelectWeapon(int index)
{
	if(CurrentWeapon)
	{
		CurrentWeapon->Select(false);
		PreviousWeapon = CurrentWeapon;
	}
	CurrentWeapon = Weapons[index];
	CurrentWeapon->Select(true);
	if(GunStaticMeshComponent && CurrentWeapon->WeaponStaticMesh)
	{
		GunStaticMeshComponent->SetStaticMesh(CurrentWeapon->WeaponStaticMesh);
	}
}

void UWeaponHandlerActorComponent::SelectUnlockedWeapon(int index)
{
	if(CurrentWeapon)
	{
		CurrentWeapon->Select(false);
		PreviousWeapon = CurrentWeapon;
	}
	CurrentWeapon = UnlockedWeapons[index];
	CurrentWeapon->Select(true);
	if(GunStaticMeshComponent && CurrentWeapon->WeaponStaticMesh)
	{
		GunStaticMeshComponent->SetStaticMesh(CurrentWeapon->WeaponStaticMesh);
	}
}

void UWeaponHandlerActorComponent::ScrollWeapon(int delta)
{
	delta = FMath::Clamp(delta, -1, 1);
	for (int i = 0; i < UnlockedWeapons.Num(); i++)
	{
		if(UnlockedWeapons[i] != CurrentWeapon) continue;

		int index = i + delta;
		if (index < 0)
		{
			index = UnlockedWeapons.Num() - 1;
		}
		if(index >= UnlockedWeapons.Num())
		{
			index = 0;
		}

		SelectUnlockedWeapon(index);
		
		OnEquipWeapon.Broadcast(CurrentWeapon);
		return;
	}
}

bool UWeaponHandlerActorComponent::HasAccessToWeapon(TObjectPtr<AWeapon> weapon)
{
	return weapon->IsUnlocked;
}

void UWeaponHandlerActorComponent::AddBomb()
{
	BombCount++;
	BombCount = FMath::Clamp(BombCount, 0, BombLimit);
}

bool UWeaponHandlerActorComponent::CanPickBombs()
{
	return BombCount >= BombLimit;
}

void UWeaponHandlerActorComponent::SetBomb(int Count)
{
	BombCount = Count;
}

void UWeaponHandlerActorComponent::StartThrow()
{
	if(BombCount <= 0) return;
	
	ThrowForce = 0;
	OnStartAim.Broadcast();
}

void UWeaponHandlerActorComponent::AimThrow(float DeltaTime)
{
	if(BombCount <= 0) return;

	TObjectPtr<APlayerController> Pc = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), 0);
	if (!Pc) return;

	FVector MousePos;
	FVector MouseDir;
	Pc->DeprojectMousePositionToWorld(MousePos, MouseDir);
	FVector PosUnderCursor = FMath::RayPlaneIntersection(MousePos, MouseDir,
		FPlane(GetOwner()->GetActorLocation(), FVector::UpVector));

	float DistanceToCursor = FVector::Dist(MyPlayer->WorldCursor->GetActorLocation(), MyPlayer->GetActorLocation());
	
	ThrowForce += DeltaTime * ForceIncreaseSpeed;
	ThrowForce = FMath::Clamp(DistanceToCursor, 0, MaxThrowForce);
	OnAim.Broadcast();
}

void UWeaponHandlerActorComponent::ThrowBomb()
{
	if(BombCount <= 0) return;
	if(ThrowForce <= MaxThrowForce / ForceAcceptanceMultiplier)
	{
		OnThrowFailed.Broadcast();
		return;
	} // return if force too low
	
	FVector SpawnLoc = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 100 + FVector(0.f, 0.f, 50);
	FRotator Rotation = (GetOwner()->GetActorForwardVector() * 100 + FVector(0.f, 0.f, BombThrowOffset)).Rotation();
	TObjectPtr<AMistBomb> Bomb = GetWorld()->SpawnActor<AMistBomb>(BombClass, SpawnLoc, Rotation);
	Bomb->Launch(ThrowForce);

	OnThrow.Broadcast(Bomb);

	BombCount--;
}
