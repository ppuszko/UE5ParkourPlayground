


#include "AnimNotifies/ANSDisableMovement.h"

void UANSDisableMovement::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	CharacterRef = Cast<ACharacterBase>(MeshComp->GetOwner());

	if (CharacterRef)
	{
		CharacterRef->SetCanMove(false);
		
	}
}

void UANSDisableMovement::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (CharacterRef)
	{
		CharacterRef->SetCanMove(true);
		CharacterRef->GetAttackComponent()->ResetAttackState();
	}
}