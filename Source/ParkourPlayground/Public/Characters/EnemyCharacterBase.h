

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/Navigation/PatrolRoute.h"
#include "BehaviorTree/BehaviorTree.h"

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


	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool GetIsAttacking() const { return AttackComponent != nullptr ? AttackComponent->GetIsAttacking() : false; }

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SwitchToChase() { GetCharacterMovement()->MaxWalkSpeed = RunningWalkSpeed; }

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SwitchToWalk() { GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed; }

	UFUNCTION(BlueprintCallable, Category = "AI")
	UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	FVector GetAndUpdatePatrolLocation();

	void ManageHUDDisplay(bool ShouldDisplay);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD")
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

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Movement")
	APatrolRoute* PatrolRoute;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;

protected:
	UFUNCTION()
	void OnHealthChanged();

	virtual void OnDeath() override;

	virtual void OnAttackFinished() override;

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;



	

};
