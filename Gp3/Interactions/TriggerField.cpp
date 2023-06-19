#include "TriggerField.h"

ATriggerField::ATriggerField()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	RootComponent = BoxCollider;
}

void ATriggerField::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATriggerField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATriggerField::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	OnOverlapBegin(OtherActor);
}

void ATriggerField::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	OnOverlapEnd(OtherActor);
}

