#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShowUIWidget.generated.h"

class UImage;

UCLASS()
class GP3_API UShowUIWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> MyImage;
};
