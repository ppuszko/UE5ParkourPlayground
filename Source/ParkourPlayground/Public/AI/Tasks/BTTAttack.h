

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTAttack.generated.h"

/**
 * 
 */
UCLASS()
class PARKOURPLAYGROUND_API UBTTAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, Category = "AI|Keys")
	FName AttackDurationKey = "AttackDuration";

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
