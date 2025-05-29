


#include "AI/Tasks/BTT_TaskRNG.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"

EBTNodeResult::Type UBTT_TaskRNG::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* AIController = OwnerComp.GetAIOwner())
	{
		if (UBlackboardComponent* BB = AIController->GetBlackboardComponent())
		{
			BB->SetValueAsFloat(RandomValueKey, UKismetMathLibrary::RandomFloatInRange(0.f, 1.f));
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}