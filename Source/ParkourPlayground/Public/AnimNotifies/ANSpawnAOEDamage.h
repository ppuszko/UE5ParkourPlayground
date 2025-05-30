

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ANSpawnAOEDamage.generated.h"

/**
 *
 */
UCLASS(Blueprintable, meta = (DisplayName = "SpawnAOEDamage"))
class PARKOURPLAYGROUND_API UANSpawnAOEDamage : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
