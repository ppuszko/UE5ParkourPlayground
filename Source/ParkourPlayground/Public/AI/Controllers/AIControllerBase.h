

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "AIControllerBase.generated.h"

/**
 * 
 */
class UBehaviorTree;
class UAIPerceptionComponent;
class UAISenseConfig_Sight;

UCLASS()
class PARKOURPLAYGROUND_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()
	
public:
	AAIControllerBase();

protected:
	UFUNCTION()
	void OnSightPerceptionUpdated(AActor* SourceActor, FAIStimulus Stimulus);

	void OnTimer();

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* AIPerception;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAISenseConfig_Sight* SightConfig;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	float ChaseTimeAfterLostSight = 4.f;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Keys")
	FName LineOfSightKey;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Keys")
	FName TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "AI|Keys")
	FName AttackCountKey = "AttackCount";

	float TimeElapsed = 0.f;

	FTimerHandle LineOfSightTimer;



protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

};
