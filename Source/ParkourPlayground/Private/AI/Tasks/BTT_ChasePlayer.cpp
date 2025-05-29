


#include "AI/Tasks/BTT_ChasePlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Characters/EnemyCharacterBase.h"


EBTNodeResult::Type UBTT_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed; 
		
	AEnemyCharacterBase* ControlledPawn = Cast<AEnemyCharacterBase>(AIController->GetPawn());
	if (!ControlledPawn) return EBTNodeResult::Failed;
	
	ControlledPawn->SwitchToChase();
	return EBTNodeResult::Succeeded;
}