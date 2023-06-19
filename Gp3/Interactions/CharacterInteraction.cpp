#include "CharacterInteraction.h"
#include "Interactable.h"
#include "Components/ShapeComponent.h"

UCharacterInteraction::UCharacterInteraction()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCharacterInteraction::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(DelayHandle, this, &UCharacterInteraction::BindDelegates, 1.f);
}

void UCharacterInteraction::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(bAutoPickup)
	{
		CheckInteraction();
	}
}

bool UCharacterInteraction::CheckInteraction()
{
	TArray<AActor*> actors;
	OverlapArea->GetOverlappingActors(actors, AInteractable::StaticClass());
	if(actors.IsEmpty()) return false;

	Cast<AInteractable>(actors[0])->Interact(this);
	OnPickItem.Broadcast(Cast<AInteractable>(actors[0]));
	return true;
}

void UCharacterInteraction::BindDelegates()
{
	if(!OverlapArea || bAutoPickup) return;
	OverlapArea->OnComponentBeginOverlap.AddDynamic(this, &UCharacterInteraction::ShowUIPrompt);
	OverlapArea->OnComponentEndOverlap.AddDynamic(this, &UCharacterInteraction::HideUIPrompt);
}

void UCharacterInteraction::ShowUIPrompt(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TObjectPtr<AInteractable> Interactable = Cast<AInteractable>(OtherActor);
	if(!Interactable) return;
	Interactable->ShowUIPrompt();
}

void UCharacterInteraction::HideUIPrompt(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	TObjectPtr<AInteractable> Interactable = Cast<AInteractable>(OtherActor);
	if(!Interactable) return;
	Interactable->HideUIPrompt();
}

