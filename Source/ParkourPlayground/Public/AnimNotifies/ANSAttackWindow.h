

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANSAttackWindow.generated.h"

class ACharacterBase;
/**
 * 
 */
UCLASS(Blueprintable, meta = (DisplayName = "ContinueAttackWindow"))
class PARKOURPLAYGROUND_API UANSAttackWindow : public UAnimNotifyState
{
	GENERATED_BODY()

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	ACharacterBase* CharacterRef;

};
