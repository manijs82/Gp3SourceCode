// Fill out your copyright notice in the Description page of Project Settings.

#include "ShowUIField.h"
#include "ShowUIWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

AShowUIField::AShowUIField()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AShowUIField::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if(WasTriggered) return;

	if(OtherActor != UGameplayStatics::GetPlayerPawn(GetWorld(), 0)) return; //Didn't overlap with player

	GetWorld()->GetTimerManager().SetTimer(ShowUITimerHandle, this, &AShowUIField::FadeInUI, GetWorld()->GetDeltaSeconds(), true);
	
	WasTriggered = true;
}

void AShowUIField::FadeInUI()
{
	TimeElapsed += GetWorld()->GetTimerManager().GetTimerElapsed(ShowUITimerHandle);
	if(TimeElapsed >= FadeTime) //Has finished fading in
	{
		GetWorld()->GetTimerManager().ClearTimer(ShowUITimerHandle);
		GetWorld()->GetTimerManager().SetTimer(ShowUITimerHandle, this, &AShowUIField::WaitWhileUIIsShown, TimeToShowImage);
	}

	//Create widget if it doesn't currently exist
	if(!CreatedWidget)
	{
		CreatedWidget = Cast<UShowUIWidget>(CreateWidget(GetWorld(), ShowUIWidgetTemplate));
		CreatedWidget->AddToViewport();
		CreatedWidget->MyImage->SetBrushFromTexture(ImageToShow);
		CreatedWidget->SetRenderOpacity(0.f);
	}

	//Fade widget in
	float Percentage = TimeElapsed / FadeTime;
	CreatedWidget->SetRenderOpacity(Percentage);
}

void AShowUIField::WaitWhileUIIsShown()
{
	//Fade widget out
	GetWorld()->GetTimerManager().SetTimer(ShowUITimerHandle, this, &AShowUIField::FadeOutUI, GetWorld()->GetDeltaSeconds(), true);
}

void AShowUIField::FadeOutUI()
{
	TimeElapsed -= GetWorld()->GetTimerManager().GetTimerElapsed(ShowUITimerHandle);
	if(TimeElapsed <= 0.f) //Has finished fading out
	{
		CreatedWidget->RemoveFromParent();
		GetWorld()->GetTimerManager().ClearTimer(ShowUITimerHandle);
	}

	//Fade widget out
	float Percentage = TimeElapsed / FadeTime;
	CreatedWidget->SetRenderOpacity(Percentage);
}