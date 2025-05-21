

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskBase.generated.h"

/**
 * 
 */
UCLASS()
class PARKOURPLAYGROUND_API UBTTaskBase : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI|Keys")
	FName LineOfSightKey = TEXT("HasLineOfSight");

	UPROPERTY(EditDefaultsOnly, Category = "AI|Keys")
	FName TargetActorKey = TEXT("TargetActor");


	UPROPERTY(EditAnywhere, Category = "AI|Keys")
	FName TargetLocationKey = TEXT("TargetLocation");

};
