


#include "AnimNotifies/ANSToggleInterruptible.h"
#include "Characters/CharacterBase.h"

void UANSToggleInterruptible::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		Character->SetIsInterruptible(false);
	}
}

void UANSToggleInterruptible::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		Character->SetIsInterruptible(true);
	}
}
