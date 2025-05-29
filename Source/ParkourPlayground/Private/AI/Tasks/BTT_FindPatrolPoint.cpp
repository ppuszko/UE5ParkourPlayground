

#include "AI/Tasks/BTT_FindPatrolPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Characters/EnemyCharacterBase.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

EBTNodeResult::Type UBTT_FindPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	AEnemyCharacterBase* ControlledPawn = Cast<AEnemyCharacterBase>(AIController->GetPawn());

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

	if (ControlledPawn && BB)
	{
		ControlledPawn->SwitchToWalk();
		BB->SetValueAsVector(TargetLocationKey, ControlledPawn->GetAndUpdatePatrolLocation());

		return EBTNodeResult::Succeeded;
	}
	else return EBTNodeResult::Failed;
}