#pragma once

#include "CoreMinimal.h"
#include "TriggerField.h"
#include "ShowUIField.generated.h"

class UShowUIWidget;

UCLASS()
class GP3_API AShowUIField : public ATriggerField
{
	GENERATED_BODY()

	FTimerHandle ShowUITimerHandle;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UShowUIWidget> ShowUIWidgetTemplate;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> ImageToShow;

	UPROPERTY(EditAnywhere)
	float TimeToShowImage;
	
	UPROPERTY(EditAnywhere)
	float FadeTime;
	
public:
	AShowUIField();
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	bool WasTriggered = false;
	float TimeElapsed = 0.f;

	UPROPERTY()
	TObjectPtr<UShowUIWidget> CreatedWidget;

	void FadeInUI();
	void WaitWhileUIIsShown();
	void FadeOutUI();
};
