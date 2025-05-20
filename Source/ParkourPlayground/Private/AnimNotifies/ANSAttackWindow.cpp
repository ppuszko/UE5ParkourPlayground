


#include "AnimNotifies/ANSAttackWindow.h"
#include "Characters/CharacterBase.h"
#include "ActorComponents/AttackComponent.h"
#include "Animation/AnimNotifyLibrary.h"

void UANSAttackWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	CharacterRef = Cast<ACharacterBase>(MeshComp->GetOwner());
	if(CharacterRef)
	{
		CharacterRef->GetAttackComponent()->AllowComboContinuation();
	}
}

void UANSAttackWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (CharacterRef && UAnimNotifyLibrary::NotifyStateReachedEnd(EventReference))
	{
		CharacterRef->GetAttackComponent()->OnAttackFinished.Broadcast();
		CharacterRef->GetAttackComponent()->ResetAttackState();
	}
}
