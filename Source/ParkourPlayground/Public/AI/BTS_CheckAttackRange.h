

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_CheckAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class PARKOURPLAYGROUND_API UBTS_CheckAttackRange : public UBTService
{
	GENERATED_BODY()
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;

	void PerformCheck(UBehaviorTreeComponent& OwnerComp);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange;

	UPROPERTY(EditAnywhere, Category = "AI|Keys")
	FName IsInRangeKey = TEXT("IsInAttackRange");

	UPROPERTY(EditAnywhere, Category = "AI|Keys")
	FName TargetActorKey = TEXT("TargetActor");

	UPROPERTY(EditAnywhere, Category = "AI|Keys")
	FName CanMoveKey = TEXT("CanMove");


};
