


#include "AnimNotifies/ANSRoll.h"
#include "Characters/PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"

void UANSRoll::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner()))
	{
		Player->ToggleInvincibility(true);
	}
}

void UANSRoll::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner()))
	{
		Player->ToggleInvincibility(false);
		Player->GetCameraBoom()->bUsePawnControlRotation = true;  
		Player->GetCameraBoom()->bInheritYaw = true;
		Player->GetCameraBoom()->bInheritRoll = true;
		Player->GetCameraBoom()->bInheritPitch = true;
	}
}
