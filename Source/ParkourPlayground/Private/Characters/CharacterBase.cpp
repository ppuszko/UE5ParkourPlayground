


#include "Characters/CharacterBase.h"
#include "Weapons/WeaponBase.h"
#include "Components/SkinnedMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "DataTypes/DataTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ACharacterBase::ACharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Stats = CreateDefaultSubobject<UCharacterStatsComponent>(TEXT("StatsComponent"));
	AttackComponent = CreateDefaultSubobject<UAttackComponent>(TEXT("AttackComponent"));
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);

	SetGenericTeamId(FGenericTeamId(static_cast<uint8>(EAffiliation::Neutral)));
	CanMove = true;
	IsInterruptible = true;
}

// Start IDamageable interface //

bool ACharacterBase::TakeDamage(AActor* Causer, bool HitReactRight)
{
	ACharacterBase* DamageCauser = Cast<ACharacterBase>(Causer);
	FSDamageInfo DamageInfo = DamageCauser->GetAttackComponent()->ConstructDamageInfo();

	return Stats->TakeDamage(DamageInfo, Causer, HitReactRight);
}

bool ACharacterBase::ShouldResolveAttackCollision(AActor* AttackingActor)
{
	if (!GetTeamAttitudeTowards(*AttackingActor) == ETeamAttitude::Friendly)
	{
		return true;
	}
	return false;
}

bool ACharacterBase::CalculateHitDirection(FVector HitLocation, FVector& ResultingHitDirection)
{
	FVector HitDirection = (GetActorLocation() - HitLocation);
	HitDirection.Z = 0;
	HitDirection.Normalize();

	ResultingHitDirection = HitDirection;

	FVector HitActorForwardVector = GetActorForwardVector();
	HitActorForwardVector.Z = 0;

	return UKismetMathLibrary::Cross_VectorVector(HitActorForwardVector, HitDirection).Z > 0 ? false : true;
}

// Finish IDamageable interface //

void ACharacterBase::StartPiercingDash()
{
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	UCapsuleComponent* Collider = GetCapsuleComponent();

	if (MoveComp)
	{
		MoveComp->SetMovementMode(EMovementMode::MOVE_Flying);
		if (Collider)
		{
			Collider->SetCollisionResponseToAllChannels(ECR_Ignore);
			Collider->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
			Collider->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
		}	
	}
}


void ACharacterBase::EndPiercingDash()
{
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	UCapsuleComponent* Collider = GetCapsuleComponent();

	if (MoveComp)
	{
		MoveComp->SetMovementMode(EMovementMode::MOVE_Walking);
		if (Collider)
		{
			Collider->SetCollisionResponseToAllChannels(ECR_Block);
			Collider->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
			Collider->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
		}
	}
}


//temporary
void ACharacterBase::SwapWeapons()
{
	CurrWeaponIndex++;
	if (CurrWeaponIndex > 2) CurrWeaponIndex = 0;

	TSubclassOf<AWeaponBase> ToSpawn;
	if (CurrWeaponIndex == 1) ToSpawn = SpareWeaponClass;
	else if (CurrWeaponIndex == 2) ToSpawn = SpareWeaponClass2;
	else if (CurrWeaponIndex == 0) ToSpawn = WeaponClass;

	Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Weapon->Destroy();
	Weapon = nullptr;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	Weapon = GetWorld()->SpawnActor<AWeaponBase>(ToSpawn, FVector(), FRotator(), SpawnParams);

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
}

// Start IGenericTeamAgentInterface // 

void ACharacterBase::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	CharacterTeamId = TeamID;
}

FGenericTeamId ACharacterBase::GetGenericTeamId() const
{
	return CharacterTeamId;
}

ETeamAttitude::Type ACharacterBase::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IGenericTeamAgentInterface* OtherAgent = Cast<const IGenericTeamAgentInterface>(&Other);
	const IGenericTeamAgentInterface* ThisAgent = Cast<const IGenericTeamAgentInterface>(this);
	if (OtherAgent && ThisAgent)
	{
		return OtherAgent->GetGenericTeamId() == ThisAgent->GetGenericTeamId() ? ETeamAttitude::Friendly : ETeamAttitude::Hostile;
	}
	return ETeamAttitude::Neutral;
}

// Finish IGenericTeamAgentInterface // 

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

	if (AttackComponent)
	{
		AttackComponent->OnAttackFinished.AddDynamic(this, &ACharacterBase::OnAttackFinished);
		AttackComponent->OnAttackStarted.AddDynamic(this, &ACharacterBase::OnAttackStarted);
	}

	if (Stats)
	{
		Stats->OnDeath.AddDynamic(this, &ACharacterBase::OnDeath);
		Stats->OnDamaged.AddDynamic(this, &ACharacterBase::OnDamaged);
	}


	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ACharacterBase::OnAttackStarted()
{
	CanMove = false;
}

void ACharacterBase::OnAttackFinished()
{
	if (!AttackComponent->GetIsAttacking()) CanMove = true;
}

void ACharacterBase::OnDamaged(EDamageResponse DamageResponse, AActor* Causer, bool HitReactRight)
{
	if (IsInterruptible)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (DamageResponse == EDamageResponse::Default)
		{
			if (HitReactLeftMontage && HitReactRightMontage && AnimInstance)
			{
				if (HitReactRight)
				{
					AnimInstance->Montage_Play(HitReactRightMontage);
				}
				else
				{
					AnimInstance->Montage_Play(HitReactLeftMontage);
				}
			}
		}
		else if (DamageResponse == EDamageResponse::Knockback)
		{
			if (KnockbackMontage && AnimInstance)
			{
				AnimInstance->Montage_Play(KnockbackMontage);

				//FRotator NewRot = Causer->GetActorRotation();
				//NewRot.Yaw += 180.f;
				////NewRot.Normalize();

				//SetActorRotation(Causer->GetActorRotation());

				LaunchCharacter(Causer->GetActorForwardVector() * 3000.f, true, false);
			}
		}
	}
}

void ACharacterBase::OnDeath()
{
	if (DeathAnim)
	{
		PlayAnimMontage(DeathAnim);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
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

