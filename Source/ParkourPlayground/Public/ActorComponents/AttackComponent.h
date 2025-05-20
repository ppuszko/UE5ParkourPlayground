

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

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

	void AllowComboContinuation() { CanAttack = true; }

	UFUNCTION(BlueprintCallable, Category = "AttackSystem")
	void Attack();

	UFUNCTION(BlueprintCallable, Category = "AttackSystem")
	int GetCurrentAttacIndex() const { return AttackIndex; }

	UFUNCTION(BlueprintCallable, Category = "AttackSystem")
	bool GetIsAttacking() const { return IsAttacking; }

	UFUNCTION(BlueprintCallable, Category = "AttackSystem")
	void SetCanQueryCombo(bool bQueryCombo) { CanQueryCombo = bQueryCombo; }

	UFUNCTION(BlueprintCallable, Category = "AttackSystem")
	void PlayAttackMontage();

	UFUNCTION(BlueprintCallable, Category = "AttackSystem")
	void ResetAttackState() { IsAttacking = false; AttackIndex = 0; }

	UFUNCTION(BlueprintCallable, Category = "AttackSystem")
	void InitializeWhenOwnerIsReady();

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
	bool CanQueryCombo = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AttackSystem")
	bool CanAttack = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AttackSystem")
	int AttackIndex = 0;

	UPROPERTY(VisibleAnywhere, BLueprintReadOnly, Category = "AttackSystem");
	int AttackCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AttackSystem")
	ACharacterBase* OwningCharacter;



	// Called when the game starts
	virtual void BeginPlay() override;

};
