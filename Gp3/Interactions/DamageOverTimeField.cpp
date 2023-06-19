#include "DamageOverTimeField.h"

#include "Gp3/Enemy/EnemyBase.h"

ADamageOverTimeField::ADamageOverTimeField()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADamageOverTimeField::BeginPlay()
{
	Super::BeginPlay();

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(DamageTimerHandle, this, &ADamageOverTimeField::DamageHealths, DamageRate, true);
}

void ADamageOverTimeField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADamageOverTimeField::NotifyActorBeginOverlap(AActor* OtherActor)
{
	TObjectPtr<UHealthActorComponent> HealthComponent = Cast<UHealthActorComponent>(OtherActor->GetComponentByClass(UHealthActorComponent::StaticClass())); //If actor has health component
	if(HealthComponent)
	{
		Healths.Add(HealthComponent);
	}
	
	Super::NotifyActorBeginOverlap(OtherActor);
}

void ADamageOverTimeField::NotifyActorEndOverlap(AActor* OtherActor)
{
	TObjectPtr<UHealthActorComponent> HealthComponent = Cast<UHealthActorComponent>(OtherActor->GetComponentByClass(UHealthActorComponent::StaticClass())); //If actor has health component
	if(HealthComponent && Healths.Contains(HealthComponent))
	{
		Healths.Remove(HealthComponent);
	}
	
	Super::NotifyActorEndOverlap(OtherActor);
}

void ADamageOverTimeField::DamageHealths()
{
	if(bIsDisabled) return;
	if(!Healths.IsEmpty())
	{
		for (UHealthActorComponent* Health : Healths)
		{
			if(Health->GetOwner()->IsA(AEnemyBase::StaticClass()) && !bCanDamageEnemies) break;
			Health->UpdateHealthBy(-DamageAmount);
			OnDamage();
		}
	}
}

void ADamageOverTimeField::DisableDamaging()
{
	if(!bCanDisable) return;
	bIsDisabled = true;
	OnDisabled();
}

