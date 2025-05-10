


#include "Weapons/WeaponBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Characters/CharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameplayTagAssetInterface.h"
#include "Interfaces/Damageable.h"

void AWeaponBase::ToggleActive()
{
	if (IsActive)
	{
		CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CapsuleComponent->SetHiddenInGame(true);
	}
	else
	{
		CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		CapsuleComponent->SetHiddenInGame(false);
	}
	IsActive = !IsActive;
}

// Sets default values
AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateOptionalDefaultSubobject<UStaticMeshComponent>("WeaponMesh");

	RootComponent = Mesh;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("WeaponCollider");
	CapsuleComponent->InitCapsuleSize(0.f, 0.f);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	//CapsuleComponent->SetCollisionProfileName(TEXT("Trigger"));
	CapsuleComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AWeaponBase::HandleOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* WeaponOwner = Cast<ACharacterBase>(OverlappedComp->GetOwner()->GetOwner());

	if (WeaponOwner && WeaponOwner != OtherActor)
	{
		if (HitActors.Contains(OtherActor)) { return; }
		else
		{
			if (CanBeAttacked(OtherActor, WeaponOwner))
			{
				HitActors.Add(OtherActor);
				if (IDamageable* ActorToHit = Cast<IDamageable>(OtherActor))
				{

					ActorToHit->TakeDamage(WeaponOwner);
				}

			}
			

		}
	}
}

FSDamageInfo AWeaponBase::ConstructDamageInfo(int Index)
{
	const UAttackDataAsset* AttackData = GetAttackDataByIndex(Index);
	checkf(AttackData, TEXT("Attack Data is null"));

	FSDamageInfo DamageInfo{WeaponDataAsset->BaseDamage, AttackData->DamageResponse, WeaponDataAsset->DamageType,
							AttackData->CanDamageInvincible, AttackData->CanBeBlocked, AttackData->CanForceInterrupt};

	return DamageInfo;
}

bool AWeaponBase::CanBeAttacked(AActor* TargetActor, AActor* OwningActor)
{
	IGameplayTagAssetInterface* Target = Cast<IGameplayTagAssetInterface>(TargetActor);
	IGameplayTagAssetInterface* Causer = Cast<IGameplayTagAssetInterface>(OwningActor);

	if (Target && Owner)
	{
		FGameplayTagContainer TargetTags;
		Target->GetOwnedGameplayTags(TargetTags);

		FGameplayTagContainer CauserTags;
		Causer->GetOwnedGameplayTags(CauserTags);

		return !CauserTags.HasAny(TargetTags);
	}
	return false;
}


