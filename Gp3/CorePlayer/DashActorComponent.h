// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DashActorComponent.generated.h"

class AGp3Character;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartDash);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndDash);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GP3_API UDashActorComponent : public UActorComponent
{
	GENERATED_BODY()

	FTimerHandle DashCooldownHandle;

	UPROPERTY(EditAnywhere, Category = Variables)
	TObjectPtr<UCurveFloat> DashCurve;

	UPROPERTY(EditAnywhere, Category = Variables)
	float DashTime = 0.5f;

	UPROPERTY(EditAnywhere, Category = Variables)
	float DashCooldownTime = 0.4f;

public:
	UDashActorComponent();

	void AttemptDash();
	UFUNCTION(BlueprintCallable)
	void CancelDash();
	UPROPERTY(BlueprintAssignable)
	FStartDash OnStartDash;
	UPROPERTY(BlueprintAssignable)
	FEndDash OnEndDash;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TObjectPtr<AGp3Character> MyPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variables)
	float MaxDashDistance = 400.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variables)
	FVector2D DashCheckOffsets;

	UPROPERTY(BlueprintReadOnly)
	float DashDistance;
	float TimeElapsed;
	bool bIsDashFirstTick;
	UPROPERTY(BlueprintReadOnly)
	bool bIsDashing;
	void Dash(float DeltaSeconds);
	void DashCooldown();
	float GetDashDistance(float bottomZ, float topZ, float rayCount);

	UPROPERTY(BlueprintReadOnly)
	FVector StartLoc;
	UPROPERTY(BlueprintReadOnly)
	FVector StartDir;
};
