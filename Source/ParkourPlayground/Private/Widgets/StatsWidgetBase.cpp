


#include "Widgets/StatsWidgetBase.h"

void UStatsWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
}

void UStatsWidgetBase::SetHealth(float Percent)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(FMath::Clamp(Percent, 0.0f, 1.0f));
	}
}
