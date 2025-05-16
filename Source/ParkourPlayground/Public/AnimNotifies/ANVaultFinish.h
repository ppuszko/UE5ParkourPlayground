

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ANVaultFinish.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, meta = (DisplayName = "FinishVault"))
class PARKOURPLAYGROUND_API UANVaultFinish : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
