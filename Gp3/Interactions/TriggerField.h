// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "TriggerField.generated.h"

UCLASS()
class GP3_API ATriggerField : public AActor
{
	GENERATED_BODY()
	
public:
	ATriggerField();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnOverlapBegin(AActor* OtherActor);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnOverlapEnd(AActor* OtherActor);

	UPROPERTY(EditAnywhere, Category = Variables, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> BoxCollider;
};
