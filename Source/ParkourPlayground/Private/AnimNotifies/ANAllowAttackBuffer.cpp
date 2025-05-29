


#include "AnimNotifies/ANAllowAttackBuffer.h"
#include "Characters/CharacterBase.h"

void UANAllowAttackBuffer::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if(ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		Character->GetAttackComponent()->SetCanBufferAttack(true);
	}
}