

#include "AI/BTS_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Characters/CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBTS_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	PerformCheck(OwnerComp);
}

void UBTS_CheckAttackRange::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);

	PerformCheck(SearchData.OwnerComp);
}

void UBTS_CheckAttackRange::PerformCheck(UBehaviorTreeComponent& OwnerComp)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AAIController* AIController = OwnerComp.GetAIOwner();
	ACharacterBase* ControlledPawn = Cast<ACharacterBase>(AIController->GetPawn());

	auto* TargetObject = BB->GetValueAsObject(TargetActorKey);
	AActor* TargetActor = Cast<AActor>(TargetObject);

	if (BB && AIController && ControlledPawn && TargetActor)
	{
		float DistanceToTarget = (TargetActor->GetActorLocation() - ControlledPawn->GetActorLocation()).Length();
		BB->SetValueAsBool(IsInRangeKey, DistanceToTarget <= AttackRange);
		BB->SetValueAsBool(CanMoveKey, ControlledPawn->GetCanMove());
	}
	else BB->SetValueAsBool(IsInRangeKey, false);
}
