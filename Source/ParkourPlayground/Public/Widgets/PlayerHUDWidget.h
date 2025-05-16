

#pragma once

#include "CoreMinimal.h"
#include "Widgets/StatsWidgetBase.h"
#include "PlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class PARKOURPLAYGROUND_API UPlayerHUDWidget : public UStatsWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	void SetStamina(float Percent);

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaBar;

};
