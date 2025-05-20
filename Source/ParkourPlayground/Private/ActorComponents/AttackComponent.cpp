


#include "ActorComponents/AttackComponent.h"
#include "Characters/CharacterBase.h"
#include "Weapons/WeaponBase.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UAttackComponent::UAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UAttackComponent::Attack()
{ 
	if (CanAttack)
	{
		if (IsAttacking)
		{
			CanAttack = false;
			AttackIndex++;
			if (AttackIndex > AttackCount) AttackIndex = 0;
			PlayAttackMontage();

		}
		else
		{
			CanAttack = false;
			IsAttacking = true;
			PlayAttackMontage();
		}
	}
}

void UAttackComponent::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = OwningCharacter->GetMesh()->GetAnimInstance();
	if (!OwningCharacter->GetWeapon()) return;
	UAnimMontage* MontageToPlay = OwningCharacter->GetWeapon()->GetAttackDataByIndex(AttackIndex)->AttackMontage;

	if (AnimInstance != nullptr && MontageToPlay != nullptr)
	{
		CanAttack = false;
		//AnimInstance->StopAllMontages(.2f);
		AnimInstance->Montage_Play(MontageToPlay, 1.f);
		OnAttackStarted.Broadcast();
	}
}

void UAttackComponent::InitializeWhenOwnerIsReady()
{
	OwningCharacter = Cast<ACharacterBase>(GetOwner());

	ensureMsgf(OwningCharacter, TEXT("Actor: %s is not a valid AttackSystemOwner!"), *GetOwner()->GetName());
	ensureMsgf(OwningCharacter->GetWeapon(), TEXT("Actor %s weapon in nullptr"), *GetOwner()->GetName());

	if (OwningCharacter != nullptr)
	{
		if (OwningCharacter->GetWeapon() != nullptr)
		{
			AttackCount = OwningCharacter->GetWeapon()->GetAttackCount() - 1;
		}
	}

}


// Called when the game starts
void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();
}

