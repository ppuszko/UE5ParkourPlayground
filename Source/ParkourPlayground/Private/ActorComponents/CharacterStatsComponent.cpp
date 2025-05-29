


#include "ActorComponents/CharacterStatsComponent.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UCharacterStatsComponent::UCharacterStatsComponent()
{
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

bool UCharacterStatsComponent::TakeDamage(FSDamageInfo DamageInfo, AActor* Causer, bool HitReactRight)
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
			
			if (CurrentHealth <= 0.f)
			{
				OnDeath.Broadcast();
				IsDead = true;
			}
			else
			{
				OnDamaged.Broadcast(DamageInfo.DamageResponse, GetOwner(), HitReactRight);
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


