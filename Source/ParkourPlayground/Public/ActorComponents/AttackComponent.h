

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapons/Data/AttackDataAsset.h"
#include "AttackComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttack);


class ACharacterBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PARKOURPLAYGROUND_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAttackComponent();

	void AllowComboContinuation();

	UFUNCTION(BlueprintCallable, Category = "AttackSystem")
	void Attack();

	UFUNCTION(BlueprintCallable, Category = "AttackSystem")
	void SpecialAttack(int Index);

	UFUNCTION(BlueprintCallable, Category = "AttackSystem")
	int GetCurrentAttacIndex() const { return AttackIndex; }

	UFUNCTION(BlueprintCallable, Category = "AttackSystem")
	const UAttackDataAsset* GetCurrentSpecialAttack() const { return CurrentSpecialAttack; }

	UFUNCTION(BlueprintCallable, Category = "AttackSystem")
	int GetSpecialAttacksCount() const { return SpecialAttacks.Num(); }

	UFUNCTION(BlueprintCallable, Category = "AttackSystem")
	FSDamageInfo ConstructDamageInfo();

	UFUNCTION(BlueprintCallable, Category = "AttackSystem")
	bool GetIsAttacking() const { return IsAttacking; }

	UFUNCTION(BlueprintCallable, Category = "AttackSystem")
	bool GetCanAttack() const { return CanAttack; }

	UFUNCTION(BlueprintCallable, Category = "AttackSystem")
	float GetAttackTime() const { return CurrentAttackMontage ? (CurrentAttackMontage->GetPlayLength() * CurrentAttackMontage->RateScale) - 0.1f : 0.f; }

	UFUNCTION(BlueprintCallable, Category = "AttackSystem")
	int GetAttackCount() const { return AttackCount; }

	UFUNCTION(BlueprintCallable, Category = "AttackSystem")
	void SetCanQueryCombo(bool bQueryCombo) { CanQueryCombo = bQueryCombo; }

	UFUNCTION(BlueprintCallable, Category = "AttackSystem")
	void LoadCurrentWeaponAttack();

	UFUNCTION(BlueprintCallable, Category = "AttackSystem")
	void LoadSpecialAttack(int Index);

	UFUNCTION(BlueprintCallable, Category = "AttackSystem")
	void PlayAttackMontage();

	UFUNCTION(BlueprintCallable, Category = "AttackSystem")
	void ResetAttackState();

	UFUNCTION(BlueprintCallable, Category = "AttackSystem")
	void InitializeWhenOwnerIsReady();

	UFUNCTION(BlueprintCallable, Category = "AttackSystem")
	void SetCanBufferAttack(bool bCanBufferAttack) { CanBufferAttack = bCanBufferAttack; }

public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAttack OnAttackFinished;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAttack OnAttackStarted;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AttackSystem")
	bool IsAttacking = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AttackSystem")
	bool ContinueCombo = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AttackSystem")
	bool IsCurrentAttackSpecial = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AttackSystem")
	bool CanQueryCombo = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AttackSystem")
	bool CanAttack = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AttackSystem")
	bool CanBufferAttack = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AttackSystem")
	bool HasBufferedAttack = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AttackSystem")
	int AttackIndex;

	UPROPERTY(VisibleAnywhere, BLueprintReadOnly, Category = "AttackSystem");
	int AttackCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AttackSystem")
	ACharacterBase* OwningCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AttackSystem")
	UAnimMontage* CurrentAttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AttackSystem")
	TArray<UAttackDataAsset*> SpecialAttacks;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AttackSystem")
	UAttackDataAsset* CurrentSpecialAttack;

	// Called when the game starts
	virtual void BeginPlay() override;

};
