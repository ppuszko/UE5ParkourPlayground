


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

	AttackIndex = 0;
	// ...
}

void UAttackComponent::AllowComboContinuation()
{
	CanAttack = true; 
	CanBufferAttack = false;
	if (HasBufferedAttack) Attack();
	HasBufferedAttack = false;
}

void UAttackComponent::Attack()
{ 
	if (CanAttack)
	{
		IsCurrentAttackSpecial = false;
		CanAttack = false;
		if (IsAttacking)
		{
			AttackIndex++;
			if (AttackIndex > AttackCount) AttackIndex = 0;
		}
		else
		{
			IsAttacking = true;
		}
		LoadCurrentWeaponAttack();
		PlayAttackMontage();
	}
	else if (CanBufferAttack)
	{
		HasBufferedAttack = true;
	}
}

void UAttackComponent::SpecialAttack(int Index)
{
	if (CanAttack)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can attack"))
		IsCurrentAttackSpecial = true;
		CanAttack = false;
		IsAttacking = true;
		LoadSpecialAttack(Index);
		PlayAttackMontage();
	}
}

FSDamageInfo UAttackComponent::ConstructDamageInfo()
{
	FSDamageInfo DamageInfo;
	if (IsCurrentAttackSpecial)
	{
		DamageInfo.DamageAmount = CurrentSpecialAttack->SpecialDamage;
		DamageInfo.DamageResponse = CurrentSpecialAttack->DamageResponse;
		DamageInfo.DamageType = OwningCharacter->GetWeapon()->GetWeaponDataAsset()->DamageType;
		DamageInfo.CanDamageInvincible = CurrentSpecialAttack->CanDamageInvincible;
		DamageInfo.CanBeBlocked = CurrentSpecialAttack->CanBeBlocked;
		DamageInfo.CanForceInterrupt = CurrentSpecialAttack->CanForceInterrupt;
	}
	else
	{
		DamageInfo = OwningCharacter->GetWeapon()->ConstructDamageInfo(AttackIndex);
	}

	return DamageInfo;
}

void UAttackComponent::LoadCurrentWeaponAttack()
{
	if (!OwningCharacter->GetWeapon())
	{
		CurrentAttackMontage = nullptr;
		return;
	}

	CurrentAttackMontage = OwningCharacter->GetWeapon()->GetAttackDataByIndex(AttackIndex)->AttackMontage;
}

void UAttackComponent::LoadSpecialAttack(int Index)
{
	if (Index >= 0 && Index < SpecialAttacks.Num())
	{
		CurrentAttackMontage = SpecialAttacks[Index]->AttackMontage;
		CurrentSpecialAttack = SpecialAttacks[Index];
	}
	else
	{
		CurrentAttackMontage = nullptr;
		CurrentSpecialAttack = nullptr;
	}
}

void UAttackComponent::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = OwningCharacter->GetMesh()->GetAnimInstance();

	if (AnimInstance != nullptr && CurrentAttackMontage != nullptr)
	{
		AnimInstance->Montage_Play(CurrentAttackMontage);
		OnAttackStarted.Broadcast();
	}
}

void UAttackComponent::ResetAttackState()
{
	IsAttacking = false; 
	AttackIndex = 0;
	CanAttack = true;
	CanBufferAttack = false;
	HasBufferedAttack = false;
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

