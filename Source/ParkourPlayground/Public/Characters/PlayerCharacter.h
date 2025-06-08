

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "Materials/MaterialInterface.h"


#include "PlayerCharacter.generated.h"

/**
 *
 */

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;
class UStatsWidgetBase;
class UPlayerHUDWidget;
class USphereComponent;
class AEnemyCharacterBase;
class UMotionWarpingComponent;

UENUM(BlueprintType)
enum class EVaultType : uint8
{
	Close = 0,
	Far,
	CloseHigh,
	FarHigh
};

UCLASS()
class PARKOURPLAYGROUND_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TargetFocus")
	bool IsTargetLocked;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetMovementMode(ECollisionResponse Response = ECollisionResponse::ECR_Block, EMovementMode Mode = EMovementMode::MOVE_Walking, bool ShouldTestCameraCollision = true);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetIsRolling(bool bIsRolling) { IsRolling = bIsRolling; }

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetIsVaulting(bool bIsVaulting) { IsVaulting = bIsVaulting; }

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool GetIsVaulting() const { return IsVaulting; }

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetHasVaulted(bool bHasVaulted) { HasVaulted = bHasVaulted; }

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool GetIsAttacking() const { return AttackComponent != nullptr ? AttackComponent->GetIsAttacking() : false; }

	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetHighFallVelocityThreshold() const { return HighFallVelocityThreshold; }

	virtual void ToggleInvincibility(bool Invincible) override;

	AEnemyCharacterBase* FocusedObject;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USphereComponent* EnemyDetectionRadius;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MotionWarping", meta = (AllowPrivateAccess = "true"))
	UMotionWarpingComponent* MotionWarping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LightAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RollAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* TargetLockAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SwapWeaponAction;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD")
	UPlayerHUDWidget* Widget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UStatsWidgetBase> WidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float WalkMoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintMoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool IsSprinting;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool IsVaulting;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montages")
	UAnimMontage* RollMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = "Montages")
	TMap<EVaultType, UAnimMontage*> VaultMontages;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Movement")
	EVaultType VaultType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float CurrentStamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float MaxStamina;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = "TargetFocus")
	UMaterialInterface* HighlightMaterial;

	UPROPERTY(EditAnywhere, Category="Collision")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_WorldStatic;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TSubclassOf<UCameraShakeBase> LandingCameraShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TSubclassOf<UCameraShakeBase> HardLandingCameraShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	float HighFallVelocityThreshold;

protected:

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void DrainStamina();

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void RegenStamina();

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void InitStaminaRegen();

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void InitStaminaDrain();

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void UpdateStamina();

	UFUNCTION(BlueprintCallable, Category = "Events")
	void OnDetectionRadiusBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Events")
	void OnDetectionRadiusEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, Category = "Events")
	void OnVaultFinished(UAnimMontage* Montage, bool bIsInterrupted);

	UFUNCTION(BlueprintCallable, Category = "Events")
	void OnRollFinished(UAnimMontage* Montage, bool bIsInterrupted);

	UFUNCTION(BlueprintCallable, Category = "Events")
	void OnHealthChanged();

	UFUNCTION(BlueprintCallable, Category = "TargetFocus")
	void CalculateFocusCandidate();

	UFUNCTION(BlueprintCallable, Category = "TargetFocus")
	void UpdateOverlayMatOnActor(ACharacterBase* Actor, bool ShouldSet);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsNearObstacle();

	virtual void OnAttackStarted() override;

	virtual void OnAttackFinished() override;

	virtual void OnDeath() override;

protected:


	virtual void Move(const FInputActionValue& Value);
	virtual void Look(const FInputActionValue& Value);
	virtual void LightAttack(const FInputActionValue& Value);
	virtual void Sprint(const FInputActionValue& Value);
	virtual void Roll(const FInputActionValue& Value);
	virtual void JumpStart(const FInputActionValue& Value);
	virtual void JumpEnd(const FInputActionValue& Value);
	virtual void TargetLock(const FInputActionValue& Value);
	virtual void SwapWeapon(const FInputActionValue& Value);

protected:
	FTimerHandle StaminaTimer;
	float StaminaDrainRate;
	float StaminaRegenRate;
	float StaminaAttackCost;
	TArray<AEnemyCharacterBase*> EnemiesInRange;
	FVector VaultStart;
	FVector VaultEnd;
	bool HasVaulted;
	
protected:

	virtual void BeginPlay() override;

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;

	virtual void Landed(const FHitResult& Hit) override;


};
