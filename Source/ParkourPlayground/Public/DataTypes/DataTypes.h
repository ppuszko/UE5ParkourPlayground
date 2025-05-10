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
	Elemental
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