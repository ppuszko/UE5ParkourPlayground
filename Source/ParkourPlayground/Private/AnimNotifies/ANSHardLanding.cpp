


#include "AnimNotifies/ANSHardLanding.h"
#include "Characters/PlayerCharacter.h"

void UANSHardLanding::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if(APlayerCharacter * Player = Cast<APlayerCharacter>(MeshComp->GetOwner()))
	{
		Player->SetCanMove(false);
	}
}

void UANSHardLanding::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner()))
	{
		Player->SetCanMove(true);
	}
}