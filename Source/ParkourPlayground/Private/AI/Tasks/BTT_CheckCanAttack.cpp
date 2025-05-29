


#include "AI/Tasks/BTT_CheckCanAttack.h"
#include "AIController.h"
#include "Characters/EnemyCharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UBTT_CheckCanAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* AIController = OwnerComp.GetAIOwner())
	{
		if (AEnemyCharacterBase* ControlledPawn = Cast<AEnemyCharacterBase>(AIController->GetPawn()))
		{
			UAttackComponent* AttackComp = ControlledPawn->GetAttackComponent();
			UBlackboardComponent* BB = AIController->GetBlackboardComponent();

			if (AttackComp && BB)
			{
				BB->SetValueAsBool(CanAttackKey, AttackComp->GetCanAttack());
				BB->SetValueAsInt(AttackCountKey, AttackComp->GetAttackCount());
				return EBTNodeResult::Succeeded;
			}
			
		}
	}
	return EBTNodeResult::Failed;
}
