#pragma once

#include "CoreMinimal.h"
#include "DataTypes.generated.h"

UENUM(BlueprintType)
enum class EDamageResponse : uint8
{
	Default,
	Stun,
	Knockback,
};

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	Default,
	Bleed,
	Fire, 
	Poison
};

USTRUCT(BlueprintType)
struct FSDamageInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	EDamageResponse DamageResponse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	EDamageType DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	bool CanDamageInvincible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	bool CanBeBlocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	bool CanForceInterrupt;
};

USTRUCT(BlueprintType)
struct FSStatusEffects
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float CurrentFireStatus;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status")
	float MaxFireStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float CurrentPoisonStatus;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status")
	float MaxPoisonStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float CurrentBleedStatus;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status")
	float MaxBleedStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float CurrentStaggerStatus;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status")
	float MaxStaggerStatus;

};