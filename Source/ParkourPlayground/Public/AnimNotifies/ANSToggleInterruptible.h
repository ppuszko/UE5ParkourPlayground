

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANSToggleInterruptible.generated.h"

/**
 * 
 */
UCLASS()
class PARKOURPLAYGROUND_API UANSToggleInterruptible : public UAnimNotifyState
{
	GENERATED_BODY()
	
  virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	
  virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};
