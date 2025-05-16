


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
		if (IsAttacking && !ContinueCombo)
		{
			ContinueCombo = true;
		}
		else
		{
			UAnimInstance* AnimInstance = OwningCharacter->GetMesh()->GetAnimInstance();



			if (AnimInstance != nullptr)
			{
				if (UAnimMontage* MontageToPlay = OwningCharacter->GetWeapon()->GetAttackDataByIndex(AttackIndex)->AttackMontage)
				{
					UE_LOG(LogTemp, Warning, TEXT("Entered AnimMontage"));
					IsAttacking = true;

					AnimInstance->StopAllMontages(.2f);

					AnimInstance->Montage_Play(MontageToPlay, 1.f);

					OnAttackStarted.Broadcast();
					CanAttack = false;
				}
			}
		}
	}
}

void UAttackComponent::EvaluateCombo()
{
	if (ContinueCombo)
	{
		AttackIndex++;
		if (AttackIndex > AttackCount) AttackIndex = 0;


		Attack();
		ContinueCombo = false;
	}
	else
	{
		AttackIndex = 0;
		IsAttacking = false;
		OnAttackFinished.Broadcast();
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

