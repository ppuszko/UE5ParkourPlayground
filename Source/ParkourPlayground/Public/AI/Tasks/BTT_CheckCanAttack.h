

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_CheckCanAttack.generated.h"

/**
 * 
 */
UCLASS()
class PARKOURPLAYGROUND_API UBTT_CheckCanAttack : public UBTTaskNode
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "AI|Keys")
	FName CanAttackKey = "CanAttack";

	UPROPERTY(EditAnywhere, Category = "AI|Keys")
	FName AttackCountKey = "AttackCount";

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	

};
