

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "AI/Controllers/AIControllerBase.h"
#include "Components/WidgetComponent.h"


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

	void ManageHUDDisplay(bool ShouldDisplay);

protected:
	UFUNCTION()
	void OnDeath();

	UFUNCTION()
	void OnHealthChanged();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	TSubclassOf<AAIControllerBase> AICClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="HUD")
	UWidgetComponent* HealthWidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD")
	UStatsWidgetBase* Widget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UStatsWidgetBase> WidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	bool ShouldDisplayHUD;


	virtual void Tick(float DeltaTime) override;

};
