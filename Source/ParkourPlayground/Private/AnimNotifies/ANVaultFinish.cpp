


#include "AnimNotifies/ANVaultFinish.h"
#include "Characters/PlayerCharacter.h"

void UANVaultFinish::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner()))
	{
		Player->SetMovementMode();
		Player->SetHasVaulted(false);
	}
}
