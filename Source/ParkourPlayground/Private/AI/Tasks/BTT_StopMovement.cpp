


#include "AI/Tasks/BTT_StopMovement.h"
#include "AIController.h"


EBTNodeResult::Type UBTT_StopMovement::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner());

	if (AIController)
	{
		AIController->StopMovement();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}