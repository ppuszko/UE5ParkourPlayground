


#include "Weapons/WeaponBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

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
	AActor* WeaponOwner = Cast<AActor>(OverlappedComp->GetOwner()->GetOwner());
	if (WeaponOwner && WeaponOwner != OtherActor)
	{
		if (HitActors.Contains(OtherActor)) { return; }
		else
		{
			HitActors.Add(OtherActor);

			//TODO: implement receiving damage when IDamageable is implemented
		}
	}
}


