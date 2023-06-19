// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Gp3Character.generated.h"

class UCharacterInteraction;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class UWeaponHandlerActorComponent;
class UHealthActorComponent;
class UDashActorComponent;
struct FInputActionInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUseHealthPack);

UCLASS(Blueprintable)
class AGp3Character : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCharacterInteraction> InteractionActorComponent;


public:
	AGp3Character();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UHealthActorComponent> HealthActorComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UWeaponHandlerActorComponent> WeaponHandlerActorComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UDashActorComponent> DashActorComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> WorldCursorClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CursorZOffset = 50;
	
	FORCEINLINE TObjectPtr<UCameraComponent> GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	FORCEINLINE TObjectPtr<USpringArmComponent> GetCameraBoom() const { return CameraBoom; }
	UFUNCTION(BlueprintCallable)
	void SetLookingAngle(float angleDegrees);

	UPROPERTY(BlueprintReadWrite)
	bool bPlayerHasControl = true;
	UPROPERTY(BlueprintReadWrite)
	bool bCanTakeMovementInput = true;
	
	//Input functions
	void Move(const FInputActionInstance& Instance);
	void SetLookDirection(const FInputActionInstance& Instance);
	void StartWeaponUse();
	void TriggerWeaponUse();
	void EndWeaponUse();
	void AttemptInteract();
	void CallWeaponAction(int index);
	void InteractOrReload();
	void SelectWeapon(int index);
	void ScrollWeapon(const FInputActionInstance& Instance);
	void AngleTowardsReticle(const FInputActionInstance& Instance);
	float DistanceToCursor;
	FVector MovingDirection;
	FVector CurrentLookingDirection;
	FVector2D TargetLookingDirection;
	void AttemptDash();
	void AttemptCrouch();
	void AttemptUncrouch();
	void AttemptHeal();
	void AttemptStartThrow();
	void AttemptAim();
	void AttemptEndAim();

	UPROPERTY(EditAnywhere)
	float UncrouchedHeight = 96.f;
	UPROPERTY(EditAnywhere)
	float CrouchHeight = 32.f;

	UPROPERTY(EditAnywhere)
	float HealthpackHealAmount = 100.f;
	UPROPERTY(BlueprintReadWrite)
	int HealthpackAmount = 0;
	UPROPERTY(BlueprintReadOnly)
	float CurrentLookingAngle = 0;
	
	UPROPERTY(BlueprintAssignable)
	FUseHealthPack OnUseHealthPack;

	UPROPERTY()
	TObjectPtr<AActor> WorldCursor;
	FVector2D MoveDir;
	UFUNCTION(BlueprintCallable)
	FVector2D GetMoveDir();
	
private:
	//Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> TopDownCameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UShapeComponent> OverlapArea;

	//Input
	virtual void BeginPlay() override;
	void RotateTowardsTargetRotation(float DeltaSeconds);
	virtual void Tick(float DeltaSeconds) override;
	FVector GetCameraRelativeDirection(const FVector& Direction) const;
	FVector2D GetCameraRelativeDirection(const FVector2D& Direction) const;
};

