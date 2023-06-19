// Fill out your copyright notice in the Description page of Project Settings.

#include "DashActorComponent.h"
#include "Gp3/Gp3Character.h"

UDashActorComponent::UDashActorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDashActorComponent::BeginPlay()
{
	Super::BeginPlay();

	MyPlayer = Cast<AGp3Character>(GetOwner());
}

void UDashActorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(bIsDashFirstTick)
	{
		OnStartDash.Broadcast();
		bIsDashFirstTick = false;
	}
	if(bIsDashing)
	{
		Dash(DeltaTime);
	}
}

void UDashActorComponent::AttemptDash()
{
	if(GetWorld()->GetTimerManager().IsTimerActive(DashCooldownHandle)) return; //In cooldown
	
	TimeElapsed = 0.f;
	bIsDashing = true;
	bIsDashFirstTick = true;

	StartLoc = MyPlayer->GetActorLocation();
	StartDir = MyPlayer->GetVelocity().Length() < 0.1f ? MyPlayer->GetActorForwardVector() : MyPlayer->MovingDirection;
	
	DashDistance = GetDashDistance(DashCheckOffsets.X, DashCheckOffsets.Y, 5);
	
	GetWorld()->GetTimerManager().SetTimer(DashCooldownHandle, this, &UDashActorComponent::DashCooldown, DashCooldownTime);
}

float UDashActorComponent::GetDashDistance(float bottomZ, float topZ, float rayCount)
{
	TArray<FHitResult> Hits;
	
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, MyPlayer);

	for (int i = 0; i < rayCount; i++)
	{
		FHitResult Hit;
		FVector Start = StartLoc + FVector::UpVector * FMath::Lerp(bottomZ, topZ, i/(rayCount - 1));
		
		FVector LineTraceEnd = Start + StartDir * MaxDashDistance;
		GetWorld()->LineTraceSingleByChannel(
		OUT Hit,
		Start,
		LineTraceEnd,
		ECC_Visibility,
		TraceParams);

		Hits.Add(Hit);
	}

	float Dist = MaxDashDistance;
	for (FHitResult Hit : Hits)
	{
		if(!Hit.bBlockingHit) continue;
		if(Hit.Distance < Dist)
		{
			Dist = Hit.Distance - 50;
		}
	}

	return Dist;
}

void UDashActorComponent::CancelDash()
{
	if(bIsDashing)
	{
		TimeElapsed = DashTime;
	}
}

void UDashActorComponent::Dash(float DeltaSeconds)
{
	TimeElapsed += DeltaSeconds;

	float t = TimeElapsed/DashTime;
	t = FMath::Clamp(t, 0, DashDistance/MaxDashDistance);

	if(t >= DashDistance/MaxDashDistance)
	{
		bIsDashing = false;
		OnEndDash.Broadcast();
		return;
	}

	FVector NewLoc = StartLoc + StartDir * MaxDashDistance * DashCurve->GetFloatValue(t);
	if(FVector::Distance(StartLoc, NewLoc) >= DashDistance) return;
	MyPlayer->SetActorLocation(NewLoc);
	
}

void UDashActorComponent::DashCooldown()
{
	GetWorld()->GetTimerManager().ClearTimer(DashCooldownHandle);
}