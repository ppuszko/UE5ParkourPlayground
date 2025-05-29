

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius;

	UPROPERTY(EditAnywhere, Category = "AI|Keys")
	FName TargetLocationKey = TEXT("TargetLocation");

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
