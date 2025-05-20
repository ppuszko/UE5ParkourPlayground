

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
	void OnSightPerceptionUpdated(AActor* SourceActor, FAIStimulus Stimulus);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UAIPerceptionComponent* AIPerception;

	UAISenseConfig_Sight* SightConfig;

	virtual void BeginPlay() override;

};
