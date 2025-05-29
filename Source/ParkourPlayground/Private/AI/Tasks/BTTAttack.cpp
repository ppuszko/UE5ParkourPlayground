


#include "AI/Tasks/BTTAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Characters/EnemyCharacterBase.h"

EBTNodeResult::Type UBTTAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* AIController = OwnerComp.GetAIOwner())
	{
		if (AEnemyCharacterBase* ControlledPawn = Cast<AEnemyCharacterBase>(AIController->GetPawn()))
		{
			if (UAttackComponent* AttackComp = ControlledPawn->GetAttackComponent())
			{
				UBlackboardComponent* BB = AIController->GetBlackboardComponent();
				AttackComp->Attack();
				
				if (BB) 
				{
					BB->SetValueAsFloat(AttackDurationKey, AttackComp->GetAttackTime());
				} 

				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;

}