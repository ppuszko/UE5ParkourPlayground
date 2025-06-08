

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ActorComponents/CharacterStatsComponent.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"
#include "Interfaces/Damageable.h"
#include "ActorComponents/AttackComponent.h"
#include "Weapons/WeaponBase.h"
#include "GenericTeamAgentInterface.h"


#include "CharacterBase.generated.h"


UCLASS()
class PARKOURPLAYGROUND_API ACharacterBase : public ACharacter, public IDamageable, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

	// Start IDamageable interface //

	UFUNCTION(BlueprintCallable, Category="IDamageable")
	virtual bool TakeDamage(AActor* Causer, bool HitReactRight) override;

	UFUNCTION(BlueprintCallable, Category = "IDamageable")
	virtual bool ShouldResolveAttackCollision(AActor* AttackingActor) override;

	UFUNCTION(BlueprintCallable, Category = "IDamageable")
	virtual bool CalculateHitDirection(FVector HitLocation, FVector& ResultingHitDirection) override;

	// Finish IDamageable interface //

	// Start Getters & Setters //

	// Setters
	UFUNCTION(BlueprintCallable, Category="Movement")
	void SetIsInterruptible(bool bIsInterruptible) { IsInterruptible = bIsInterruptible; }

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetCanMove(bool bCanMove) { CanMove = bCanMove; }

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetCanRoll(bool bCanRoll) { CanRoll = bCanRoll; }

	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual void SetTargetActor(AActor* InTargetActor) { TargetActor = InTargetActor; }


	// Getters
	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool GetCanMove() const { return CanMove; }

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool GetIsRolling() const { return IsRolling; }

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool GetIsDead() const { return Stats != nullptr ? Stats->GetIsDead() : false; }

	UFUNCTION(BlueprintCallable, Category = "Combat")
	UAttackComponent* GetAttackComponent() { return AttackComponent; }

	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual const AActor* GetTargetActor() const { return TargetActor; }

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AWeaponBase* GetWeapon() const { return Weapon; }

	// Finish Getters & Setters //
	
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void StartPiercingDash();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void EndPiercingDash();

	void SwapWeapons();

	// Start IGenericTeamAgentInterface // 

	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;

	virtual FGenericTeamId GetGenericTeamId() const override;

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	// Finish IGenericTeamAgentInterface //

protected:

	UFUNCTION(BlueprintCallable, Category = "Events")
	virtual void OnAttackStarted();

	UFUNCTION(BlueprintCallable, Category = "Events")
	virtual void OnAttackFinished();

	UFUNCTION(BlueprintCallable, Category = "Events")
	virtual void OnDamaged(EDamageResponse DamageResponse, AActor* Causer, bool HitReactRight);

	UFUNCTION(BlueprintCallable, Category = "Events")
	virtual void OnDeath();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Team")
	FGenericTeamId CharacterTeamId;

	UPROPERTY(VisibleAnywhere, Instanced, BlueprintReadOnly, Category="Components")
	UCharacterStatsComponent* Stats;

	UPROPERTY(VisibleAnywhere, Instanced, BlueprintReadOnly, Category="Components")
	UAttackComponent* AttackComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Weapon")
	AWeaponBase* Weapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	TSubclassOf<AWeaponBase> WeaponClass;

	//temporary
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AWeaponBase> SpareWeaponClass;

	//temporary
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AWeaponBase> SpareWeaponClass2;

	//temporary
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int CurrWeaponIndex = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	FName WeaponSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montages")
	UAnimMontage* DeathAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montages")
	UAnimMontage* HitReactLeftMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montages")
	UAnimMontage* HitReactRightMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montages")
	UAnimMontage* KnockbackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	USoundBase* HitSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool CanMove;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool IsRolling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool IsInterruptible;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool CanRoll;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	AActor* TargetActor = nullptr;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
