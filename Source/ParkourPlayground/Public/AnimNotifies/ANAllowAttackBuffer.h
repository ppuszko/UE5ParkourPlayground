

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ANAllowAttackBuffer.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, meta = (DisplayName = "AllowAttackBuffer"))
class PARKOURPLAYGROUND_API UANAllowAttackBuffer : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
