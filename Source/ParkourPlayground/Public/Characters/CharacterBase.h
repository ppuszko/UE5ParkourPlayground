// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ActorComponents/CharacterStatsComponent.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"
#include "Interfaces/Damageable.h"

#include "CharacterBase.generated.h"

class UAttackComponent;
class AWeaponBase;
//something

UCLASS()
class PARKOURPLAYGROUND_API ACharacterBase : public ACharacter, public IGameplayTagAssetInterface, public IDamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	AWeaponBase* GetWeapon() const { return Weapon; }

	UFUNCTION(BlueprintCallable, Category="Components")
	virtual bool TakeDamage(AActor* Causer) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UCharacterStatsComponent* Stats;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UAttackComponent* AttackComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Weapon")
	AWeaponBase* Weapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	TSubclassOf<AWeaponBase> WeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	FName WeaponSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Team")
	FGameplayTagContainer TeamTags;

public:

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& OutTags) const override { OutTags = TeamTags; }

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
