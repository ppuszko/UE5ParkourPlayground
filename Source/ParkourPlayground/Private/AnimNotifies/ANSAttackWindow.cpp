


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
		CharacterRef->SetCanRoll(true);
	}
}

void UANSAttackWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (CharacterRef)
	{
		if (UAnimNotifyLibrary::NotifyStateReachedEnd(EventReference))
		{
			CharacterRef->GetAttackComponent()->ResetAttackState();
		}
		CharacterRef->GetAttackComponent()->OnAttackFinished.Broadcast();
	}
	
	
}
