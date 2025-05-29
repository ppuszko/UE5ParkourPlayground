


#include "AI/Tasks/BTT_SpecialAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Characters/EnemyCharacterBase.h"
#include "Kismet/KismetMathLibrary.h"

EBTNodeResult::Type UBTT_SpecialAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if(AAIController * AIController = OwnerComp.GetAIOwner())
	{
		if(AEnemyCharacterBase* ControlledPawn = Cast<AEnemyCharacterBase>(AIController->GetPawn()))
		{
			if (UAttackComponent* AttackComp = ControlledPawn->GetAttackComponent())
			{
				int SpecialAttackIndex = UKismetMathLibrary::RandomIntegerInRange(First, Last);
				AttackComp->SpecialAttack(SpecialAttackIndex);

				AIController->GetBlackboardComponent()->SetValueAsFloat(AttackDurationKey, AttackComp->GetAttackTime());

				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}