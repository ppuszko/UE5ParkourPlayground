// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataTypes/DataTypes.h"

#include "AttackDataAsset.generated.h"

/**
 * 
 */


UCLASS(Blueprintable, BlueprintType)
class PARKOURPLAYGROUND_API UAttackDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "AttackData")
	UAnimMontage* GetAttackMontage() { return AttackMontage; }

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attack Data")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack Data");
	EDamageResponse DamageResponse;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack Data");
	bool CanBeBlocked;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack Data");
	bool CanDamageInvincible;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack Data");
	bool CanForceInterrupt;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack Data");
	bool IsSpecial = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack Data");
	float SpecialAreaOfEffect = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack Data");
	float SpecialDamage = 0.f;
};
