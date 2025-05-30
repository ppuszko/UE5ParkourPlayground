


#include "Characters/EnemyCharacterBase.h"
#include "Widgets/StatsWidgetBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


#include "AI/Controllers/AIControllerBase.h"

AEnemyCharacterBase::AEnemyCharacterBase() : ACharacterBase()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HeathBarComponent"));
	HealthWidgetComponent->SetupAttachment(RootComponent);

	HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	HealthWidgetComponent->SetDrawSize(FVector2D(300.f, 50.f));
	HealthWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 120.f));
	HealthWidgetComponent->SetVisibility(false);

	DefaultWalkSpeed = 300.f;
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
	RunningWalkSpeed = 500.f;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	bUseControllerRotationYaw = true;

	ShouldDisplayHUD = false;

	SetGenericTeamId(FGenericTeamId(static_cast<uint8>(EAffiliation::Hostile)));
}

void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (HealthWidgetComponent)
	{
		HealthWidgetComponent->SetWidgetClass(WidgetClass);
		HealthWidgetComponent->InitWidget();

		Widget = Cast<UStatsWidgetBase>(HealthWidgetComponent->GetUserWidgetObject());
		if (Widget)
		{
			Widget->SetHealth(Stats->GetHealthPercent());
		}
		HealthWidgetComponent->SetVisibility(ShouldDisplayHUD);
	}

	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (MoveComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Movement Mode: %d"), (int32)MoveComp->MovementMode);
	}

	if (Stats) Stats->OnHealthChanged.AddDynamic(this, &AEnemyCharacterBase::OnHealthChanged);

}

FVector AEnemyCharacterBase::GetAndUpdatePatrolLocation()
{
	if (PatrolRoute)
	{
		FVector Location = PatrolRoute->GetCurrentPatrolDestination();
		PatrolRoute->IncrementPatrolIndex();

		return Location;
	}
	
	return GetActorLocation();
}

void AEnemyCharacterBase::ManageHUDDisplay(bool ShouldDisplay)
{
	ShouldDisplayHUD = ShouldDisplay;

	HealthWidgetComponent->SetVisibility(ShouldDisplayHUD);
}

void AEnemyCharacterBase::OnDeath()
{
	Super::OnDeath();
	if (AAIController* AIC = Cast<AAIController>(GetController()))
	{
		AIC->UnPossess();
	}
	if (Widget)
	{
		HealthWidgetComponent->SetVisibility(false);
	}
}

void AEnemyCharacterBase::OnAttackFinished()
{
	Super::OnAttackFinished();

	if (!AttackComponent->GetIsAttacking()) TargetActor = nullptr;
}

void AEnemyCharacterBase::OnHealthChanged()
{
	if (Widget)
	{
		Widget->SetHealth(Stats->GetHealthPercent());
	}
}

void AEnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ShouldDisplayHUD)
	{
		if (APlayerCameraManager* CamMan = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0))
		{
			auto CamLocation = CamMan->GetTransform().GetLocation();

			auto Location = GetActorLocation();
			Location.Z = CamLocation.Z;

			HealthWidgetComponent->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(Location, CamLocation));
		}
	}
}
