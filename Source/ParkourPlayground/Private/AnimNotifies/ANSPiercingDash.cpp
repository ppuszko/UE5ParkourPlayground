


#include "AnimNotifies/ANSPiercingDash.h"
#include "Characters/CharacterBase.h"


void UANSPiercingDash::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	ACharacterBase* Owner = Cast<ACharacterBase>(MeshComp->GetOwner());

	if (Owner)
	{
		Owner->StartPiercingDash();
	}
}

void UANSPiercingDash::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	ACharacterBase* Owner = Cast<ACharacterBase>(MeshComp->GetOwner());

	if (Owner)
	{
		Owner->EndPiercingDash();
	}
}