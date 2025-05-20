


#include "AnimNotifies/ANAllowMovement.h"
#include "Characters/CharacterBase.h"

void UANAllowMovement::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if(ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		Character->SetCanMove(true);
		UE_LOG(LogTemp, Warning, TEXT("Can move"));
	}
}