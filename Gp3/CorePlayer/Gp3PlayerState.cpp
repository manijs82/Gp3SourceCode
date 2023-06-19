#include "Gp3PlayerState.h"

AGp3PlayerState::AGp3PlayerState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGp3PlayerState::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGp3PlayerState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

