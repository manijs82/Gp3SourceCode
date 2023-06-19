// Copyright Epic Games, Inc. All Rights Reserved.

#include "Gp3PlayerController.h"
#include "Gp3Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AGp3PlayerController::AGp3PlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void AGp3PlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<AGp3Character>(GetPawn());

	if (TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AGp3PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(SetAxisAction, ETriggerEvent::Triggered, this, &AGp3PlayerController::OnMovementAxisTriggered);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &AGp3PlayerController::StartShooting);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AGp3PlayerController::TriggerShooting);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Canceled, this, &AGp3PlayerController::EndShooting);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &AGp3PlayerController::EndShooting);
		EnhancedInputComponent->BindAction(WeaponAction1, ETriggerEvent::Started, this, &AGp3PlayerController::CallWeaponAction1);
		EnhancedInputComponent->BindAction(WeaponAction2, ETriggerEvent::Started, this, &AGp3PlayerController::CallWeaponAction2);
		EnhancedInputComponent->BindAction(WeaponAction3, ETriggerEvent::Started, this, &AGp3PlayerController::CallWeaponAction3);
		EnhancedInputComponent->BindAction(WeaponSelect1, ETriggerEvent::Started, this, &AGp3PlayerController::SelectWeapon1);
		EnhancedInputComponent->BindAction(WeaponSelect2, ETriggerEvent::Started, this, &AGp3PlayerController::SelectWeapon2);
		EnhancedInputComponent->BindAction(WeaponSelect3, ETriggerEvent::Started, this, &AGp3PlayerController::SelectWeapon3);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AGp3PlayerController::AttemptInteract);
		EnhancedInputComponent->BindAction(AngleTowardsReticleAction, ETriggerEvent::Triggered, this, &AGp3PlayerController::AngleTowardsReticle);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &AGp3PlayerController::AttemptDash);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AGp3PlayerController::AttemptCrouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AGp3PlayerController::AttemptUncrouch);
		EnhancedInputComponent->BindAction(HealAction, ETriggerEvent::Started, this, &AGp3PlayerController::AttemptHeal);
		EnhancedInputComponent->BindAction(ScrollWeapon, ETriggerEvent::Started, this, &AGp3PlayerController::OnScrollWeapon);
		EnhancedInputComponent->BindAction(Look, ETriggerEvent::Triggered, this, &AGp3PlayerController::OnLookAxisTriggered);
		EnhancedInputComponent->BindAction(InteractOrReload, ETriggerEvent::Started, this, &AGp3PlayerController::OnInteractOrReload);
		EnhancedInputComponent->BindAction(ThrowBomb, ETriggerEvent::Started, this, &AGp3PlayerController::StartThrow);
		EnhancedInputComponent->BindAction(ThrowBomb, ETriggerEvent::Triggered, this, &AGp3PlayerController::AimThrow);
		EnhancedInputComponent->BindAction(ThrowBomb, ETriggerEvent::Completed, this, &AGp3PlayerController::EndThrow);
		EnhancedInputComponent->BindAction(ThrowBomb, ETriggerEvent::Canceled, this, &AGp3PlayerController::EndThrow);
	}
}

void AGp3PlayerController::OnMovementAxisTriggered(const FInputActionInstance& Instance)
{
	PlayerCharacter->Move(Instance);
}

void AGp3PlayerController::OnLookAxisTriggered(const FInputActionInstance& Instance)
{
	PlayerCharacter->SetLookDirection(Instance);
}

void AGp3PlayerController::OnScrollWeapon(const FInputActionInstance& Instance)
{
	PlayerCharacter->ScrollWeapon(Instance);
}

void AGp3PlayerController::StartShooting()
{
	PlayerCharacter->StartWeaponUse();
}

void AGp3PlayerController::TriggerShooting()
{
	PlayerCharacter->TriggerWeaponUse();
}

void AGp3PlayerController::EndShooting()
{
	PlayerCharacter->EndWeaponUse();
}

void AGp3PlayerController::CallWeaponAction1()
{
	PlayerCharacter->CallWeaponAction(1);
}

void AGp3PlayerController::CallWeaponAction2()
{
	PlayerCharacter->CallWeaponAction(2);
}

void AGp3PlayerController::CallWeaponAction3()
{
	PlayerCharacter->CallWeaponAction(3);
}

void AGp3PlayerController::SelectWeapon1()
{
	PlayerCharacter->SelectWeapon(0);
}

void AGp3PlayerController::SelectWeapon2()
{
	PlayerCharacter->SelectWeapon(1);
}

void AGp3PlayerController::SelectWeapon3()
{
	PlayerCharacter->SelectWeapon(2);
}

void AGp3PlayerController::AttemptInteract()
{
	PlayerCharacter->AttemptInteract();
}

void AGp3PlayerController::OnInteractOrReload()
{
	PlayerCharacter->InteractOrReload();
}

void AGp3PlayerController::AngleTowardsReticle(const FInputActionInstance& Instance)
{
	PlayerCharacter->AngleTowardsReticle(Instance);
}

void AGp3PlayerController::AttemptDash()
{
	PlayerCharacter->AttemptDash();
}

void AGp3PlayerController::AttemptCrouch()
{
	PlayerCharacter->AttemptCrouch();
}

void AGp3PlayerController::AttemptUncrouch()
{
	PlayerCharacter->AttemptUncrouch();
}

void AGp3PlayerController::AttemptHeal()
{
	PlayerCharacter->AttemptHeal();
}

void AGp3PlayerController::StartThrow()
{
	PlayerCharacter->AttemptStartThrow();
}

void AGp3PlayerController::AimThrow()
{
	PlayerCharacter->AttemptAim();
}

void AGp3PlayerController::EndThrow()
{
	PlayerCharacter->AttemptEndAim();
}
