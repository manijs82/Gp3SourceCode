// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gp3/Gp3PlayerController.h"
#include "ProjectileBase.generated.h"

class UBoxComponent;

UCLASS()
class GP3_API AProjectileBase : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Variables)
	TObjectPtr<UBoxComponent> BoxCollider;

	UPROPERTY(EditAnywhere, Category = Variables)
	float ProjectileSpeed;

	UPROPERTY(EditAnywhere, Category = Variables)
	float ProjectileDamage;

public:
	AProjectileBase();

	UFUNCTION(BlueprintCallable)
	void StartProjectile(AActor* UserActor, AActor* GivenShield = nullptr);
	UPROPERTY(EditAnywhere, Category = Variables)
	float ProjectileDistanceLimit = 20000;
	UPROPERTY(EditAnywhere, Category = Variables)
	TObjectPtr<UParticleSystem> OnHitParticleSystem;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraSystem> ImpactFX;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> DecalClass;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> MyShield;

protected:
	virtual void Tick(float DeltaSeconds) override;

	TObjectPtr<AActor> ProjectilesOwner;

	TObjectPtr<AActor> HitActor;

	bool bCanMoveProjectile;
	FVector StartPos;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
