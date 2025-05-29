

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Characters/CharacterBase.h"
#include "ANSDisableMovement.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, meta = (DisplayName = "DisableMovementWindow"))
class PARKOURPLAYGROUND_API UANSDisableMovement : public UAnimNotifyState
{
	GENERATED_BODY()
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	ACharacterBase* CharacterRef;

};
