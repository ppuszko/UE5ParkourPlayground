// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CharacterBase.h"
#include "Weapons/WeaponBase.h"
#include "Components/SkinnedMeshComponent.h"
#include "Components/CapsuleComponent.h"

#include "Engine/World.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Stats = CreateDefaultSubobject<UCharacterStatsComponent>(TEXT("StatsComponent"));
	AttackComponent = CreateDefaultSubobject<UAttackComponent>(TEXT("AttackComponent"));
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
}


bool ACharacterBase::TakeDamage(AActor* Causer)
{
	ACharacterBase* DamageCauser = Cast<ACharacterBase>(Causer);
	auto DamageInfo = Weapon->ConstructDamageInfo(AttackComponent->GetCurrentAttacIndex());

	return Stats->TakeDamage(DamageInfo, Causer);
}


// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		Weapon = World->SpawnActor<AWeaponBase>(WeaponClass, FVector(), FRotator(), SpawnParams);
		ensureMsgf(Weapon, TEXT("SpawnActor returned null for %s"), *WeaponClass->GetName());
	}

	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules::SnapToTargetIncludingScale;

	if (Weapon != nullptr)
	{
		Weapon->AttachToComponent(GetMesh(), AttachmentRules, WeaponSocketName);
		Weapon->SetActorRelativeTransform(Weapon->GetWeaponSocketOffset());

		if (AttackComponent != nullptr)
		{
			AttackComponent->InitializeWhenOwnerIsReady();
		}
	}

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

