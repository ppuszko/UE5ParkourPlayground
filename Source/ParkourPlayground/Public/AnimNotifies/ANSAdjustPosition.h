

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Characters/CharacterBase.h"

#include "ANSAdjustPosition.generated.h"


/**
 * 
 */
UCLASS(Blueprintable, meta = (DisplayName = "AdjustAttackPosition"))
class PARKOURPLAYGROUND_API UANSAdjustPosition : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify")
	float DistanceToTravel = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify")
	bool ShouldMoveToTarget = true;

	const AActor* TargetActor = nullptr;
	ACharacterBase* Owner = nullptr;
	FVector OwnerLocation;
	FVector DestinedLocation;
	float Duration;
	float ElapsedTime = 0.f;

public:

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	

};
