

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"

#include "StatsWidgetBase.generated.h"


/**
 * 
 */
UCLASS()
class PARKOURPLAYGROUND_API UStatsWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

protected:

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* RootCanvas;

	UPROPERTY(meta =(BindWidget))
	UProgressBar* HealthBar;

public:
	void SetHealth(float Percent);

};
