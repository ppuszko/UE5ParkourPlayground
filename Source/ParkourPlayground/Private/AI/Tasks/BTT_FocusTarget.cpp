


#include "AI/Tasks/BTT_FocusTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

EBTNodeResult::Type UBTT_FocusTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* AIController = OwnerComp.GetAIOwner())
	{
		AActor* Target = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(TargetActorKey));

		if (ShouldSet && Target) AIController->SetFocus(Target);
		else AIController->SetFocus(nullptr);

		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}