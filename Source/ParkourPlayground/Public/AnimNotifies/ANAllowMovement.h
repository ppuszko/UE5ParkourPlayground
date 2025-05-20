

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ANAllowMovement.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, meta = (DisplayName = "AllowMovement"))
class PARKOURPLAYGROUND_API UANAllowMovement : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
