

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataTypes/DataTypes.h"

#include "CharacterStatsComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, HealthPercent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDamaged, EDamageResponse, DamageResponse, AActor*, Instigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBlocked);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PARKOURPLAYGROUND_API UCharacterStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStatsComponent();

	bool TakeDamage(FSDamageInfo DamageInfo, AActor* Instigator);
	
	UPROPERTY(BlueprintAssignable, Category="Events")

	//Event dispatchers
	FOnHealthChanged OnHealthChanged;
	FOnDeath OnDeath;
	FOnDamaged OnDamaged;
	FOnBlocked OnBlocked;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//Member variables
	UPROPERTY(VisibleAnywhere, Category = "Health")
	float CurrentHealth;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	bool IsInvincible;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	bool IsInterruptible;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	bool IsBlocking;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	bool IsDead;
		

	//helper functions
	bool CanReceiveDamage(bool CanDamageInvincible);
};
