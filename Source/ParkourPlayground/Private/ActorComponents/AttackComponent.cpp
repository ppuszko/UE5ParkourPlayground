


#include "ActorComponents/AttackComponent.h"
#include "Characters/CharacterBase.h"
#include "Weapons/WeaponBase.h"

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
		if (IsAttacking && !ContinueCombo)
		{
			ContinueCombo = true;
		}
		else
		{
			//OwningCharacter->GetMesh()->GetAnimInstance()->Montage_Play()
			IsAttacking = true;
			OwningCharacter->GetMesh()->GetAnimInstance()->StopAllMontages(.2f);
			UAnimMontage* MontageToPlay = OwningCharacter->GetWeapon()->GetAttackDataByIndex(AttackIndex)->AttackMontage;
			OwningCharacter->PlayAnimMontage(MontageToPlay);
			CanAttack = false;
		}
	}
}

void UAttackComponent::EvaluateCombo()
{
	if (ContinueCombo)
	{
		ContinueCombo = false;
		AttackIndex++;
		if (AttackIndex > AttackCount) AttackIndex = 0;
		Attack();
	}
	else
	{
		AttackIndex = 0;
		IsAttacking = false;
	}
}


// Called when the game starts
void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = Cast<ACharacterBase>(GetOwner());
	ensureMsgf(OwningCharacter, TEXT("Actor: %s is not a valid AttackSystemOwner!"), *GetOwner()->GetName());

	AttackCount = OwningCharacter->GetWeapon()->GetAttackCount() - 1;
}

