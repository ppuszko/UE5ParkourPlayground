


#include "AnimNotifies/RollBeginNotify.h"
#include "Interfaces/Damageable.h"

void URollBeginNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (IDamageable* Player = Cast<IDamageable>(MeshComp->GetOwner()))
	{
		//Player->ToggleInvincibility();
	}
}
