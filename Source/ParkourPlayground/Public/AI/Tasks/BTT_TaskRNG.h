

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_TaskRNG.generated.h"

/**
 * 
 */
UCLASS()
class PARKOURPLAYGROUND_API UBTT_TaskRNG : public UBTTaskNode
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "AI|Keys")
	FName RandomValueKey = "RandomValue";

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
