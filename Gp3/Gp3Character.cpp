// Copyright Epic Games, Inc. All Rights Reserved.

#include "Gp3Character.h"
#include "InputAction.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "CorePlayer/DashActorComponent.h"
#include "CorePlayer/HealthActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CorePlayer/WeaponHandlerActorComponent.h"
#include "Interactions/CharacterInteraction.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AGp3Character::AGp3Character()
{
	//Player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; //Don't rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	//Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; //Don't want to pull camera in when it collides with level

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;

	OverlapArea = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapArea"));
	OverlapArea->SetupAttachment(RootComponent);

	//Tick
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	//Create my components
	WeaponHandlerActorComponent = CreateDefaultSubobject<UWeaponHandlerActorComponent>(TEXT("WeaponHandlerActorComponent"));
	HealthActorComponent = CreateDefaultSubobject<UHealthActorComponent>(TEXT("HealthActorComponent"));
	InteractionActorComponent = CreateDefaultSubobject<UCharacterInteraction>(TEXT("InteractionActorComponent"));
	DashActorComponent = CreateDefaultSubobject<UDashActorComponent>(TEXT("DashActorComponent"));
}

void AGp3Character::BeginPlay()
{
	Super::BeginPlay();
	WorldCursor = GetWorld()->SpawnActor<AActor>(WorldCursorClass, FVector::Zero(), FRotator::ZeroRotator);
	InteractionActorComponent->OverlapArea = OverlapArea;
}

void AGp3Character::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FVector CursorPos = GetActorLocation() + FVector(TargetLookingDirection, 0) * DistanceToCursor + GetActorRightVector() * 10.f;
	CursorPos.Z = GetActorLocation().Z;
	WorldCursor->SetActorLocation(CursorPos);
	
	RotateTowardsTargetRotation(DeltaSeconds);
}

void AGp3Character::RotateTowardsTargetRotation(float DeltaSeconds)
{
	CurrentLookingDirection = GetActorForwardVector();
	FVector ToDirection = FVector(TargetLookingDirection.X, TargetLookingDirection.Y, 0).GetSafeNormal();

	//float angleRadians = FMath::Acos(CurrentLookingDirection.Dot(ToDirection));
	//float angleDegrees = FMath::Min(FMath::RadiansToDegrees(angleRadians), GetCharacterMovement()->RotationRate.Euler().Z * DeltaSeconds);
	//FVector axis = CurrentLookingDirection.Cross(ToDirection);
	//CurrentLookingDirection = CurrentLookingDirection.RotateAngleAxis(angleDegrees, axis).GetSafeNormal();	
	//FRotator Rotation = FRotator(0.f, CurrentLookingDirection.Rotation().Euler().Z, 0.f);

	FRotator r2 = FMath::RInterpTo(CurrentLookingDirection.Rotation(), ToDirection.Rotation(),
		DeltaSeconds, GetCharacterMovement()->RotationRate.Euler().Z);
	SetActorRotation(r2);
}

void AGp3Character::SetLookingAngle(float angleDegrees)
{
	CameraBoom->SetRelativeRotation(FRotator(CameraBoom->GetComponentRotation().Euler().Y, angleDegrees, 0));
	CurrentLookingAngle = angleDegrees;
}

void AGp3Character::Move(const FInputActionInstance& Instance)
{
	if(!bPlayerHasControl) return;
	if(!bCanTakeMovementInput) return;
	
	FVector Axis = Instance.GetValue().Get<FVector>().GetSafeNormal();
	MovingDirection = GetCameraRelativeDirection(Axis);

	MoveDir = FVector2D(MovingDirection.X, MovingDirection.Y);
	
	AddMovementInput(MovingDirection, 1.0, false);
}

FVector AGp3Character::GetCameraRelativeDirection(const FVector& Direction) const
{
	FVector Forward = CameraBoom->GetForwardVector();
	FVector Right = CameraBoom->GetRightVector();

	Forward.Z = 0;
	Right.Z = 0;
	Forward.Normalize();
	Right.Normalize();

	return Forward * Direction.X + Right * Direction.Y;
}

FVector2D AGp3Character::GetCameraRelativeDirection(const FVector2D& Direction) const
{
	FVector2D Forward = FVector2D(CameraBoom->GetForwardVector());
	FVector2D Right = FVector2D(CameraBoom->GetRightVector());

	return Forward * Direction.X + Right * Direction.Y;
}

void AGp3Character::SetLookDirection(const FInputActionInstance& Instance)
{
	if(!bPlayerHasControl) return;

	DistanceToCursor = 500.f;
	TargetLookingDirection = GetCameraRelativeDirection(Instance.GetValue().Get<FVector2D>()).GetSafeNormal();
}

void AGp3Character::AngleTowardsReticle(const FInputActionInstance& Instance)
{
	if(!bPlayerHasControl) return;
	if((Instance.GetValue().Get<FVector2D>()).SizeSquared() <= 0) return;

	TObjectPtr<APlayerController> Pc = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), 0);
	if (!Pc) return;

	FVector MousePos;
	FVector MouseDir;
	Pc->DeprojectMousePositionToWorld(MousePos, MouseDir);
	FVector PosUnderCursor = FMath::RayPlaneIntersection(MousePos, MouseDir, FPlane(GetActorLocation(), FVector::UpVector));

	DistanceToCursor = FVector::Dist(PosUnderCursor, GetActorLocation());
	FVector DirectionToCursor = (PosUnderCursor - GetActorLocation()).GetSafeNormal();
	TargetLookingDirection = FVector2D(DirectionToCursor.X, DirectionToCursor.Y);
}

void AGp3Character::StartWeaponUse()
{
	if(!bPlayerHasControl) return;
	
	WeaponHandlerActorComponent->StartUse();
}

void AGp3Character::TriggerWeaponUse()
{
	if(!bPlayerHasControl) return;
	
	WeaponHandlerActorComponent->TriggerUse();
}

void AGp3Character::EndWeaponUse()
{
	if(!bPlayerHasControl) return;
	
	WeaponHandlerActorComponent->EndUse();
}

void AGp3Character::CallWeaponAction(int index)
{
	if(!bPlayerHasControl) return;
	
	WeaponHandlerActorComponent->CallWeaponAction(index);
}

void AGp3Character::InteractOrReload()
{
	if(!bPlayerHasControl) return;

	if(!InteractionActorComponent->CheckInteraction())
	{
		CallWeaponAction(1);
	}
}

void AGp3Character::SelectWeapon(int index)
{
	if(!bPlayerHasControl) return;
	
	WeaponHandlerActorComponent->EquipWeaponByIndex(index);
}

void AGp3Character::ScrollWeapon(const FInputActionInstance& Instance)
{
	if(!bPlayerHasControl) return;

	int delta = Instance.GetValue().Get<float>();
	WeaponHandlerActorComponent->ScrollWeapon(delta);
}

void AGp3Character::AttemptInteract()
{
	if(!bPlayerHasControl) return;
	
	InteractionActorComponent->CheckInteraction();
}

void AGp3Character::AttemptDash()
{
	if(!bPlayerHasControl) return;
	
	DashActorComponent->AttemptDash();
}

void AGp3Character::AttemptCrouch()
{
	if(!bPlayerHasControl) return;

	Crouch();
	GetCapsuleComponent()->SetCapsuleHalfHeight(CrouchHeight);
	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - (UncrouchedHeight - CrouchHeight) / 2.f));
}

void AGp3Character::AttemptUncrouch()
{
	if(!bPlayerHasControl) return;
	
	UnCrouch();
	GetCapsuleComponent()->SetCapsuleHalfHeight(UncrouchedHeight);
	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + (UncrouchedHeight - CrouchHeight) / 2.f));
}

void AGp3Character::AttemptHeal()
{
	if(!bPlayerHasControl) return;
	
	if(HealthpackAmount <= 0) return;

	HealthpackAmount--;
	HealthActorComponent->UpdateHealthBy(HealthpackHealAmount);
	OnUseHealthPack.Broadcast();
}

void AGp3Character::AttemptStartThrow()
{
	if(!bPlayerHasControl) return;

	WeaponHandlerActorComponent->StartThrow();
}

void AGp3Character::AttemptAim()
{
	if(!bPlayerHasControl) return;

	WeaponHandlerActorComponent->AimThrow(GetWorld()->GetDeltaSeconds());
}

void AGp3Character::AttemptEndAim()
{
	if(!bPlayerHasControl) return;

	WeaponHandlerActorComponent->ThrowBomb();
}

FVector2D AGp3Character::GetMoveDir()
{
	FVector2D ForwardVector = FVector2D(GetActorForwardVector().X, GetActorForwardVector().Y);
	FVector2D RightVector = FVector2D(GetActorRightVector().X, GetActorRightVector().Y);

	float XVal = UKismetMathLibrary::DotProduct2D(ForwardVector, MoveDir);
	float YVal = UKismetMathLibrary::DotProduct2D(RightVector, MoveDir);

	FVector2D FinalRot = FVector2D(XVal, YVal);
	return FinalRot.GetSafeNormal();
}
