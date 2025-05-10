

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Weapons/Data/AttackDataAsset.h"
#include "DataTypes/DataTypes.h"

#include "WeaponDataAsset.generated.h"

/**
 * 
 */


UCLASS(BlueprintType)
class PARKOURPLAYGROUND_API UWeaponDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attack Data")
	EDamageType DamageType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack Data")
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack Data")
	TArray<UAttackDataAsset*> AttackStyle;

};
