


#include "Widgets/PlayerHUDWidget.h"

void UPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerHUDWidget::SetStamina(float Percent)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(FMath::Clamp(Percent, 0.f, 1.f));
	}
}