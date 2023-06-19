// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileBase.h"
#include "Components/BoxComponent.h"
#include "Gp3/CorePlayer/HealthActorComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

AProjectileBase::AProjectileBase()
{
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	RootComponent = BoxCollider;

	PrimaryActorTick.bCanEverTick = true;

	ProjectileSpeed = 100.f;
	ProjectileDamage = 30.f;
	
	bCanMoveProjectile = false;
}

void AProjectileBase::StartProjectile(AActor* UserActor, AActor* GivenShield)
{
	ProjectilesOwner = UserActor;
	MyShield = GivenShield;
	bCanMoveProjectile = true;
	StartPos = GetActorLocation();
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!bCanMoveProjectile) return;

	if(FVector::Distance(GetActorLocation(), StartPos) >= ProjectileDistanceLimit)
	{
		Destroy();
	}

	FVector NewLoc = GetActorLocation() + GetActorForwardVector() * ProjectileSpeed * DeltaTime;
	SetActorLocation(NewLoc);
}

void AProjectileBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if(!bCanMoveProjectile) return;

	if(OtherActor->IsA(AProjectileBase::StaticClass())) return; // Don't hit projectiles
	if(MyShield)
	{
		if(OtherActor == MyShield) return; //Don't collide with my own shield
	}
	if(OtherActor == ProjectilesOwner) return; //Don't damage self
	if(OtherActor == HitActor) return; //Don't hit the same enemy multiple times

	HitActor = OtherActor;
	TObjectPtr<UHealthActorComponent> HealthComponent =
		Cast<UHealthActorComponent>(OtherActor->GetComponentByClass(UHealthActorComponent::StaticClass())); //If actor has health component

	FCollisionQueryParams TraceParams(FName(TEXT("")), false, this);
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(OUT HitResult,GetActorLocation() - GetActorForwardVector() * 30,
		GetActorLocation() + GetActorForwardVector() * 30,ECC_Visibility, TraceParams);
	
	if(HealthComponent)
	{
		HealthComponent->UpdateHealthBy(-ProjectileDamage);

		if(OnHitParticleSystem) //On hit effects
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OnHitParticleSystem, GetActorLocation(), FRotator::ZeroRotator, FVector(1), true);
		}
	}
	else
	{
		if(ImpactFX)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactFX,
				HitResult.Location, HitResult.Normal.Rotation(), FVector(1), true);
		}
		if(DecalClass)
		{
			TObjectPtr<AActor> Decal = GetWorld()->SpawnActor<AActor>(DecalClass, HitResult.Location, HitResult.Normal.Rotation());
			Decal->SetActorRotation(Decal->GetActorForwardVector().RotateAngleAxis(-90, Decal->GetActorRightVector()).Rotation());
		}
	}

	Destroy();
}
