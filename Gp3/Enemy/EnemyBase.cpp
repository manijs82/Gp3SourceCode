// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyBase.h"

#include "BrainComponent.h"
#include "EnemyAIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gp3/CorePlayer/HealthActorComponent.h"
#include "Gp3/Environment/CoverPoint.h"
#include "Gp3/Weapons/SimpleGun.h"
#include "Kismet/GameplayStatics.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	HealthActorComponent = CreateDefaultSubobject<UHealthActorComponent>(TEXT("HealthActorComponent"));
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	MyWeapon = GetWorld()->SpawnActor<ASimpleGun>(MyWeaponTemplate, FVector::Zero(), FRotator::ZeroRotator);
	HealthActorComponent->OnDeath.AddDynamic(this, &AEnemyBase::Die);
	HealthActorComponent->OnHit.AddDynamic(this, &AEnemyBase::Hit);

	MyController = Cast<AEnemyAIController>(GetController());
}

void AEnemyBase::Die()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSoundCue, GetActorLocation(), GetActorRotation());
	OnDeath.Broadcast();
	Cast<AEnemyAIController>(GetController())->BrainComponent->StopLogic(FString("Died"));
	SetLifeSpan(2.f);
}

void AEnemyBase::Hit()
{
	PlaySfx(DamagedSoundCue);
	OnHit.Broadcast();
}

void AEnemyBase::Alert()
{
	MyController->Aggro();
	OnAlerted.Broadcast();
}

TObjectPtr<AActor> AEnemyBase::GetCoverPoint()
{
	return MyCoverPoint;
}

bool AEnemyBase::IsInCover()
{
	if(!MyCoverPoint) return true;
	
	FVector VectorToCoverPoint = MyCoverPoint->GetActorLocation() - GetActorLocation();
	float Distance = VectorToCoverPoint.Length();
	bool IsInCover = Distance < 100.f;
	return IsInCover;
}

void AEnemyBase::EnemyCrouch()
{
	if(bIsEnemyCrouched) return;
	GetCapsuleComponent()->SetCapsuleHalfHeight(CrouchHeight);
	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - (UncrouchedHeight - CrouchHeight) / 2.f));
	bIsEnemyCrouched = true;
}

void AEnemyBase::EnemyUnCrouch()
{
	if(!bIsEnemyCrouched) return;
	GetCapsuleComponent()->SetCapsuleHalfHeight(UncrouchedHeight);
	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + (UncrouchedHeight - CrouchHeight) / 2.f));
	bIsEnemyCrouched = false;
}

void AEnemyBase::PlaySfx(USoundCue* SfxCue)
{
	if(GetWorld()->GetTimerManager().IsTimerActive(SfxCooldownHandle)) return;
	
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SfxCue, GetActorLocation(), GetActorRotation());

	GetWorld()->GetTimerManager().SetTimer(SfxCooldownHandle, this, &AEnemyBase::SfxCooldown, SfxCooldownTime);
}

void AEnemyBase::SfxCooldown()
{
	GetWorld()->GetTimerManager().ClearTimer(SfxCooldownHandle);
}
