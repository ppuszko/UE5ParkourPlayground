

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Weapons/Data/AttackDataAsset.h"
#include "DataTypes/DataTypes.h"

#include "WeaponDataAsset.generated.h"

/**
 * 
 */


UCLASS(Blueprintable, BlueprintType)
class PARKOURPLAYGROUND_API UWeaponDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	const UAttackDataAsset* GetAttackData(int Index) const;

	UFUNCTION(BlueprintCallable)
	int GetAttackCount() const { return AttackStyle.Num(); }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack Data")
	EDamageType DamageType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack Data")
	float BaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack Data")
	TArray<UAttackDataAsset*> AttackStyle;

};
