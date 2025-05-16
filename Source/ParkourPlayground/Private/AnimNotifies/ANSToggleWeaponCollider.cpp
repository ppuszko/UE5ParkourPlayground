


#include "AnimNotifies/ANSToggleWeaponCollider.h"
#include "Characters/CharacterBase.h"
#include "Weapons/WeaponBase.h"
#include "Components/CapsuleComponent.h"

void UANSToggleWeaponCollider::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (ACharacterBase* WeaponOwner = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		if (AWeaponBase* Weapon = Cast<AWeaponBase>(WeaponOwner->GetWeapon()))
		{
			Weapon->ClearHitActors();
			Weapon->ToggleActive();
		}
	}
	
}

void UANSToggleWeaponCollider::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (ACharacterBase* WeaponOwner = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		if (AWeaponBase* Weapon = Cast<AWeaponBase>(WeaponOwner->GetWeapon()))
		{
			Weapon->ToggleActive();
		}
	}
}
