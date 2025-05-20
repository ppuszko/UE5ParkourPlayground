

#pragma once

#include "CoreMinimal.h"
#include "AI/Tasks/BTTaskBase.h"
#include "BehaviorTree/BTFunctionLibrary.h"

#include "BTT_FindPatrolPoint.generated.h"

/**
 * 
 */
UCLASS()
class PARKOURPLAYGROUND_API UBTT_FindPatrolPoint : public UBTTaskBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Radius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FBlackboardKeySelector PatrolLocation;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
