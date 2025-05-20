

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "EnemyCharacterBase.generated.h"

class UStatsWidgetBase;

/**
 *
 */
UCLASS()
class PARKOURPLAYGROUND_API AEnemyCharacterBase : public ACharacterBase
{
	GENERATED_BODY()

	AEnemyCharacterBase();

public:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool GetIsAttacking() const { return AttackComponent != nullptr ? AttackComponent->GetIsAttacking() : false; }

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SwitchToChase() { GetCharacterMovement()->MaxWalkSpeed = RunningWalkSpeed; }

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SwitchToWalk() { GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed; }

	void ManageHUDDisplay(bool ShouldDisplay);

protected:
	UFUNCTION()
	void OnDeath();

	UFUNCTION()
	void OnHealthChanged();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="HUD")
	UWidgetComponent* HealthWidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD")
	UStatsWidgetBase* Widget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UStatsWidgetBase> WidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	bool ShouldDisplayHUD;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float DefaultWalkSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float RunningWalkSpeed;


	virtual void Tick(float DeltaTime) override;

};
