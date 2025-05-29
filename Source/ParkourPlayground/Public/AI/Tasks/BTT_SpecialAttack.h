

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_SpecialAttack.generated.h"

/**
 * 
 */
UCLASS()
class PARKOURPLAYGROUND_API UBTT_SpecialAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, Category = "AI|Properties")
	int First = 0;

	UPROPERTY(EditAnywhere, Category = "AI|Properties")
	int Last = 0;

	UPROPERTY(EditAnywhere, Category = "AI|Keys")
	FName AttackDurationKey = "AttackDuration";


	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
