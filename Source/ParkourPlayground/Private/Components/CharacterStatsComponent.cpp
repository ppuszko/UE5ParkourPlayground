


#include "ActorComponents/CharacterStatsComponent.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UCharacterStatsComponent::UCharacterStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	StatusEffects.CurrentBleedStatus = 0.f;
	StatusEffects.CurrentFireStatus = 0.f;
	StatusEffects.CurrentPoisonStatus = 0.f;
	StatusEffects.CurrentStaggerStatus = 0.f;

	StatusEffects.MaxBleedStatus = 100.f;
	StatusEffects.MaxFireStatus = 100.f;
	StatusEffects.MaxPoisonStatus = 100.f;
	StatusEffects.MaxStaggerStatus = 100.f;

	CurrentHealth = 100.f;
	MaxHealth = 100.f;
	IsDead = false;
}

bool UCharacterStatsComponent::TakeDamage(FSDamageInfo DamageInfo, AActor* Causer)
{
	if (CanReceiveDamage(DamageInfo.CanDamageInvincible))
	{
		if (IsBlocking && DamageInfo.CanBeBlocked)
		{
			OnBlocked.Broadcast();
		}
		else if(!IsDead)
		{
			CurrentHealth = FMath::Clamp(CurrentHealth - DamageInfo.DamageAmount, 0.f, MaxHealth);
			
			OnDamaged.Broadcast(DamageInfo.DamageResponse, GetOwner());
			
			if (CurrentHealth <= 0.f)
			{
				OnDeath.Broadcast();
				IsDead = true;
			}

			OnHealthChanged.Broadcast();
			return true;
		}
	}
	return false;
}

bool UCharacterStatsComponent::CanReceiveDamage(bool CanDamageInvincible)
{
	return ((!IsInvincible || CanDamageInvincible) && !IsDead);
}


// Called when the game starts
void UCharacterStatsComponent::BeginPlay()
{
	Super::BeginPlay();

}


