#include "MistBomb.h"

AMistBomb::AMistBomb()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMistBomb::BeginPlay()
{
	Super::BeginPlay();
}

void AMistBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMistBomb::Launch(float Force)
{
	OnLaunch(Force);
}

