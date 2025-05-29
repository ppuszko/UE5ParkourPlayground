

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataTypes/DataTypes.h"

#include "CharacterStatsComponent.generated.h"

//
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDamaged, EDamageResponse, DamageResponse, AActor*, Instigator, bool, HitReactRight);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBlocked);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PARKOURPLAYGROUND_API UCharacterStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStatsComponent();

	bool TakeDamage(FSDamageInfo DamageInfo, AActor* Causer, bool HitReactRight);
	
	//Event dispatchers
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDeath OnDeath;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDamaged OnDamaged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnBlocked OnBlocked;

public:
	UFUNCTION(BlueprintCallable, Category="Health")
	float GetHealthPercent() const { return CurrentHealth / MaxHealth; };

	UFUNCTION(BlueprintCallable, Category="Health")
	void ToggleInvincibility(bool Invincible) { IsInvincible = Invincible; }

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool GetIsDead() const { return IsDead; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//Member variables
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float CurrentHealth;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	bool IsInvincible;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	bool IsInterruptible;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	bool IsBlocking;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	bool IsDead;
		
	UPROPERTY(EditDefaultsOnly, Category = "Status")
	FSStatusEffects StatusEffects;

	//helper functions
	bool CanReceiveDamage(bool CanDamageInvincible);
};
