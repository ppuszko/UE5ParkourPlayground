

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
	UWorld* World = OwnerComp.GetWorld();


	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

	if (ControlledPawn && World && NavSys && BB)
	{
		UE_LOG(LogTemp, Warning, TEXT("Entered condition"));
		ControlledPawn->SwitchToWalk();

		FNavLocation RandomPoint;
		if (NavSys->GetRandomReachablePointInRadius(ControlledPawn->GetActorLocation(), Radius, RandomPoint))
		{
			BB->SetValueAsVector(TargetLocationKey, RandomPoint.Location);
			AIController->MoveToLocation(RandomPoint.Location, 5.f);

			
		}
		return EBTNodeResult::Succeeded;
	}
	else return EBTNodeResult::Failed;
}