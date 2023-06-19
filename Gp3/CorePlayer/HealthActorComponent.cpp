// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthActorComponent.h"

UHealthActorComponent::UHealthActorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	MaxHealth = 100.f;
}

void UHealthActorComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}

void UHealthActorComponent::UpdateHealthBy(float Hp)
{
	if(bIsDead) return;
	if(bIsInvulnerable) return;
	
	CurrentHealth += Hp;

	CurrentHealth = FMath::Clamp(CurrentHealth, 0.f, MaxHealth);

	OnHealthUpdated.Broadcast(GetCurrentHealthPercentage());
	if(FMath::Sign(Hp) == -1)
	{
		OnHit.Broadcast();
	}

	if(CurrentHealth == 0.f)
	{
		OnDeath.Broadcast();
		bIsDead = true;
		return;
	}

	if(!bShouldNaturallyHeal) return;

	if(FMath::Sign(Hp) == -1)
	{
		GetWorld()->GetTimerManager().ClearTimer(NaturalHealHandle);
		GetWorld()->GetTimerManager().SetTimer(NaturalHealHandle, this, &UHealthActorComponent::NaturalHeal, GetWorld()->GetDeltaSeconds(), true, TimeUntilNaturalHealStarts);
	}
}

void UHealthActorComponent::NaturalHeal()
{
	UpdateHealthBy(GetWorld()->GetDeltaSeconds() * NaturalHealPower);

	if(CurrentHealth / MaxHealth >= NaturalHealLimit)
	{
		GetWorld()->GetTimerManager().ClearTimer(NaturalHealHandle);
	}
}
