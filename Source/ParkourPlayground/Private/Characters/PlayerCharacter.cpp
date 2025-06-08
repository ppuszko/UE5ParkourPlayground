


#include "Characters/PlayerCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Widgets/PlayerHUDWidget.h"
#include "Widgets/StatsWidgetBase.h"
#include "Components/SphereComponent.h"
#include "GameplayTagAssetInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Characters/EnemyCharacterBase.h"
#include "MotionWarpingComponent.h"
#include "Kismet/GameplayStatics.h"


APlayerCharacter::APlayerCharacter() : ACharacterBase()
{
	GetCapsuleComponent()->InitCapsuleSize(30.f, 90.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bIgnoreBaseRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 1000.f, 0.f);

	GetCharacterMovement()->JumpZVelocity = 800.f;
	GetCharacterMovement()->AirControl = 0.3f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.f;
	GetCharacterMovement()->GravityScale = 1.6f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 100.f);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	MotionWarping = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));

	EnemyDetectionRadius = CreateDefaultSubobject<USphereComponent>(TEXT("EnemyDetectionRadius"));
	EnemyDetectionRadius->SetSphereRadius(2000.f);
	EnemyDetectionRadius->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	EnemyDetectionRadius->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	EnemyDetectionRadius->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	EnemyDetectionRadius->SetGenerateOverlapEvents(true);
	EnemyDetectionRadius->SetupAttachment(RootComponent);

	SprintMoveSpeed = 800.f;
	WalkMoveSpeed = GetMovementComponent()->GetMaxSpeed();
	HighFallVelocityThreshold = -1000.f;

	CanMove = true;
	IsRolling = false;
	IsTargetLocked = false;
	bUseControllerRotationYaw = false;
	HasVaulted = false;
	IsVaulting = false;
	CanRoll = true;

	MaxStamina = 150.f;
	CurrentStamina = MaxStamina;
	StaminaDrainRate = 3.f;
	StaminaRegenRate = 6.f;
	StaminaAttackCost = 12.f;
}

void APlayerCharacter::ToggleInvincibility(bool Invincible)
{
	Stats->ToggleInvincibility(Invincible);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (WidgetClass)
	{
		ensureMsgf(Widget, TEXT("Widget is null"));
		Widget = CreateWidget<UPlayerHUDWidget>(GetWorld(), WidgetClass);
		if (Widget)
		{
			Widget->AddToViewport();
			Widget->SetStamina(CurrentStamina / MaxStamina);
			Widget->SetHealth(Stats->GetHealthPercent());
		}
	}

	WalkMoveSpeed = GetCharacterMovement()->GetMaxSpeed();
	IsSprinting = false;


	if (EnemyDetectionRadius)
	{
		EnemyDetectionRadius->OnComponentBeginOverlap.AddUniqueDynamic(this, &APlayerCharacter::OnDetectionRadiusBeginOverlap);
		EnemyDetectionRadius->OnComponentEndOverlap.AddUniqueDynamic(this, &APlayerCharacter::OnDetectionRadiusEndOverlap);
		//EnemyDetectionRadius->SetHiddenInGame(false);

		TArray<AActor*> OverlappingOnStart;
		EnemyDetectionRadius->GetOverlappingActors(OverlappingOnStart, AEnemyCharacterBase::StaticClass());
		for (AActor* Actor : OverlappingOnStart)
		{
			AEnemyCharacterBase* Enemy = Cast<AEnemyCharacterBase>(Actor);
			EnemiesInRange.Add(Enemy);
			Enemy->ManageHUDDisplay(true);
		}
	}

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &APlayerCharacter::OnVaultFinished);
		AnimInstance->OnMontageEnded.AddDynamic(this, &APlayerCharacter::OnRollFinished);
	}

	if (Stats) Stats->OnHealthChanged.AddDynamic(this, &APlayerCharacter::OnHealthChanged);
}

void APlayerCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(LightAttackAction, ETriggerEvent::Started, this, &APlayerCharacter::LightAttack);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &APlayerCharacter::Sprint);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::JumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopJumping);
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Started, this, &APlayerCharacter::Roll);
		EnhancedInputComponent->BindAction(TargetLockAction, ETriggerEvent::Started, this, &APlayerCharacter::TargetLock);
		EnhancedInputComponent->BindAction(SwapWeaponAction, ETriggerEvent::Started, this, &APlayerCharacter::SwapWeapon);
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CalculateFocusCandidate();

	if (IsTargetLocked)
	{
		if (FocusedObject && !GetIsDead())
		{
			if (!IsRolling)
			{
				FVector LookAtStart = CameraBoom->GetComponentLocation();
				LookAtStart -= GetActorForwardVector() * 200.f;
				LookAtStart.Z += 200.f;

				FVector LookAtEnd = FocusedObject->GetActorLocation();
				FRotator NewRot = UKismetMathLibrary::FindLookAtRotation(LookAtStart, LookAtEnd);

				FRotator SmoothedRot = FMath::RInterpTo(GetControlRotation(), NewRot, DeltaTime, 14.f);
				Controller->SetControlRotation(SmoothedRot);
			}
			else
			{
				FVector BoomLoc = CameraBoom->GetComponentLocation();
				BoomLoc.Z += 200.f;

				FVector TargetLoc = FocusedObject->GetActorLocation();
				FRotator DesiredWorldRot = (TargetLoc - BoomLoc).Rotation();
				CameraBoom->SetWorldRotation(DesiredWorldRot);
			}
		}
		else
		{
			GetCharacterMovement()->bOrientRotationToMovement = true;
			bUseControllerRotationYaw = false;

			IsTargetLocked = false;
		}
	}
}

void APlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (LandingCameraShake && HardLandingCameraShake)
	{
		if(GetCharacterMovement()->Velocity.Z >= HighFallVelocityThreshold)
			UGameplayStatics::PlayWorldCameraShake(GetWorld(), LandingCameraShake, GetActorLocation(), 400.f, 2000.f);
		else
			UGameplayStatics::PlayWorldCameraShake(GetWorld(), HardLandingCameraShake, GetActorLocation(), 400.f, 2000.f);
	}
	

}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr && CanMove)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Forward, MovementVector.Y);
		AddMovementInput(Right, MovementVector.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr && !IsTargetLocked)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::LightAttack(const FInputActionValue& Value)
{
	if (CurrentStamina >= StaminaAttackCost)
	{
		CanRoll = true;
		IsSprinting = false;
		AttackComponent->Attack();
	}

}

void APlayerCharacter::Sprint(const FInputActionValue& Value)
{
	if (!IsSprinting)
	{
		IsSprinting = true;
		InitStaminaDrain();
		GetCharacterMovement()->MaxWalkSpeed = SprintMoveSpeed;
	}
	else
	{
		IsSprinting = false;
		InitStaminaRegen();
		GetCharacterMovement()->MaxWalkSpeed = WalkMoveSpeed;
	}
}

void APlayerCharacter::Roll(const FInputActionValue& Value)
{

	if (!IsRolling && CanRoll)
	{
		IsRolling = true;
		CanRoll = false;
		AttackComponent->ResetAttackState();
		if (IsTargetLocked)
		{
			UCharacterMovementComponent* CharMovement = GetCharacterMovement();

			FVector CurrVelocity = CharMovement->Velocity;
			CurrVelocity.Z = 0;

			if (!CurrVelocity.IsNearlyZero())
			{
				FVector MoveDir = CurrVelocity.GetSafeNormal();
				FRotator NewRot = MoveDir.Rotation();

				CameraBoom->bUsePawnControlRotation = false;
				CameraBoom->SetUsingAbsoluteRotation(true);
				Controller->SetControlRotation(NewRot);


				CharMovement->bUseControllerDesiredRotation = false;
			}
		}

		if (RollMontage != nullptr)
		{
			GetMesh()->GetAnimInstance()->Montage_Play(RollMontage);
			
		}
	}
}

void APlayerCharacter::JumpStart(const FInputActionValue& Value)
{
	if ( GetVelocity().Length() > 5.f && !HasVaulted && IsNearObstacle())
	{
		if (VaultMontages.Contains(VaultType))
		{
			IsVaulting = true;
			SetMovementMode(ECollisionResponse::ECR_Ignore, EMovementMode::MOVE_Flying, false);
			PlayAnimMontage(VaultMontages[VaultType]);

			FMotionWarpingTarget MWTarget;
			MWTarget.Location = VaultStart;
			MWTarget.Rotation = GetActorRotation();
			MWTarget.Name = "VaultStart";
			MotionWarping->AddOrUpdateWarpTarget(MWTarget);

			MWTarget.Location = VaultEnd;
			MWTarget.Rotation = GetActorRotation();
			MWTarget.Name = "VaultEnd";
			MotionWarping->AddOrUpdateWarpTarget(MWTarget);
			HasVaulted = true;
		}
	}
	else
	{
		Jump();
	}
}

void APlayerCharacter::JumpEnd(const FInputActionValue& Value)
{
	if (!HasVaulted)
	{
		StopJumping();
	}
}

void APlayerCharacter::TargetLock(const FInputActionValue& Value)
{
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();

	if (IsTargetLocked)
	{
		MoveComp->bOrientRotationToMovement = true;
		bUseControllerRotationYaw = false;

		IsTargetLocked = false;
	}
	else
	{
		if (FocusedObject)
		{
			MoveComp->bOrientRotationToMovement = false;
			bUseControllerRotationYaw = true;

			IsTargetLocked = true;

			auto LookAtStart = GetActorLocation();
			LookAtStart -= GetActorForwardVector() * 200.f;
			LookAtStart.Z += 200.f;

			GetController()->SetControlRotation(UKismetMathLibrary::FindLookAtRotation(
				LookAtStart, FocusedObject->GetActorLocation()));
		}
	}
}

//temporary
void APlayerCharacter::SwapWeapon(const FInputActionValue& Value)
{
	SwapWeapons();
}

void APlayerCharacter::RegenStamina()
{
	if (!IsSprinting && !AttackComponent->GetIsAttacking())
	{
		CurrentStamina += 0.1 * StaminaRegenRate;

		if (CurrentStamina >= MaxStamina)
		{
			CurrentStamina = MaxStamina;
			GetWorld()->GetTimerManager().ClearTimer(StaminaTimer);
		}

		UpdateStamina();
	}
}

void APlayerCharacter::DrainStamina()
{
	CurrentStamina -= 0.1f * StaminaDrainRate;

	if (CurrentStamina <= 0.f || UKismetMathLibrary::VSizeXY(GetVelocity()) <= 1.f)
	{
		//CurrentStamina = 0.f;
		InitStaminaRegen();
		IsSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = WalkMoveSpeed;
	}

	UpdateStamina();
}

void APlayerCharacter::InitStaminaRegen()
{
	FTimerManager& TimeMan = GetWorld()->GetTimerManager();
	if (TimeMan.IsTimerActive(StaminaTimer))
	{
		TimeMan.ClearTimer(StaminaTimer);
	}
	TimeMan.SetTimer(StaminaTimer, this, &APlayerCharacter::RegenStamina, 0.1f, true);

}

void APlayerCharacter::InitStaminaDrain()
{
	FTimerManager& TimeMan = GetWorld()->GetTimerManager();
	if (TimeMan.IsTimerActive(StaminaTimer))
	{
		TimeMan.ClearTimer(StaminaTimer);
	}

	TimeMan.SetTimer(StaminaTimer, this, &APlayerCharacter::DrainStamina, 0.1f, true);
}

void APlayerCharacter::UpdateStamina()
{
	Widget->SetStamina(CurrentStamina / MaxStamina);
}

void APlayerCharacter::OnAttackStarted()
{
	Super::OnAttackStarted();
	CurrentStamina -= StaminaAttackCost;
	if (GetAttackComponent()->GetIsAttacking())
	{
		UpdateStamina();
	}
}

void APlayerCharacter::OnAttackFinished()
{
	Super::OnAttackFinished();
	if(!AttackComponent->GetIsAttacking()) InitStaminaRegen();
}

void APlayerCharacter::OnDeath()
{
	Super::OnDeath();
	if (APlayerController* Con = Cast<APlayerController>(GetController())) DisableInput(Con);
}

void APlayerCharacter::OnDetectionRadiusBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AEnemyCharacterBase* Enemy = Cast<AEnemyCharacterBase>(OtherActor))
	{
		//IGameplayTagAssetInterface* EnemyTags = Cast<IGameplayTagAssetInterface>(Enemy);

		if(GetTeamAttitudeTowards(*Enemy) == ETeamAttitude::Hostile)
		{
			EnemiesInRange.Add(Enemy);
			Enemy->ManageHUDDisplay(true);
		}

		//if (EnemyTags && !EnemyTags->HasMatchingGameplayTag(TeamTags.First()) && !EnemiesInRange.Contains(Enemy))
		//{
		//	
		//}
	}
}

void APlayerCharacter::OnDetectionRadiusEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AEnemyCharacterBase* Enemy = Cast<AEnemyCharacterBase>(OtherActor))
	{
		if (FocusedObject == Enemy)
		{
			UpdateOverlayMatOnActor(FocusedObject, false);
			FocusedObject = nullptr;
			TargetActor = nullptr;
		}
		EnemiesInRange.Remove(Enemy);
		Enemy->ManageHUDDisplay(false);
	}
}

void APlayerCharacter::OnVaultFinished(UAnimMontage* Montage, bool bIsInterrupted)
{
	if (VaultMontages.Contains(VaultType) && Montage == VaultMontages[VaultType])
	{
		SetMovementMode();
	}
}

void APlayerCharacter::OnRollFinished(UAnimMontage* Montage, bool bIsInterrupted)
{
	if (Montage == RollMontage)
	{
		UKismetSystemLibrary::Delay(GetWorld(), 1.5f, FLatentActionInfo());
		CameraBoom->SetUsingAbsoluteRotation(false);
		CameraBoom->bUsePawnControlRotation = true;
		CanMove = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		SetIsRolling(false);
	}
}

void APlayerCharacter::OnHealthChanged()
{
	if (Widget)
	{
		Widget->SetHealth(Stats->GetHealthPercent());
	}
}

void APlayerCharacter::CalculateFocusCandidate()
{
	if (!IsTargetLocked)
	{
		float CurrentClosestAngle = 0.f;

		for (AEnemyCharacterBase* Enemy : EnemiesInRange)
		{
			auto CamForwardVector = GetFollowCamera()->GetForwardVector();
			auto DirectionToEnemy = (Enemy->GetActorLocation() - GetActorLocation()).GetSafeNormal();

			float Dot = UKismetMathLibrary::Dot_VectorVector(CamForwardVector, DirectionToEnemy);

			if (UKismetMathLibrary::InRange_FloatFloat(Dot, 0.f, 1.f))
			{
				if (Dot >= CurrentClosestAngle)
				{
					UpdateOverlayMatOnActor(FocusedObject, false);
					FocusedObject = Enemy;
					TargetActor = Enemy;
					UpdateOverlayMatOnActor(FocusedObject, true);
					CurrentClosestAngle = Dot;
				}
			}
		}
	}
}

void APlayerCharacter::UpdateOverlayMatOnActor(ACharacterBase* Actor, bool ShouldSet)
{
	if (Actor)
	{
		if (ShouldSet)
		{
			Actor->GetMesh()->SetOverlayMaterial(HighlightMaterial);
		}
		else
		{
			Actor->GetMesh()->SetOverlayMaterial(nullptr);
		}
	}
}

bool APlayerCharacter::IsNearObstacle()
{
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	bool FoundHit = false;
	FVector TraceStart;
	FVector TraceEnd;

	for (int i = 5; i >= 0; --i)
	{
		TraceStart = GetActorLocation() + FVector(0, 0, i * 50);
		TraceEnd = TraceStart + GetActorForwardVector() * 400.f;
		FoundHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, TraceChannel, QueryParams);
		if (FoundHit)
		{
			VaultStart = HitResult.ImpactPoint;
			break;
		}
	}

	if (FoundHit)
	{
		TraceEnd = HitResult.ImpactPoint + GetActorForwardVector() * 5.f;
		TraceStart = TraceEnd + FVector(0.f, 0.f, 400.f);

		FoundHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, TraceChannel, QueryParams);

		if (FoundHit)
		{
			if (HitResult.ImpactPoint.Z - GetActorLocation().Z >= 350.f)
			{
				return false;
			}
			else
			{
				VaultStart = HitResult.ImpactPoint;
				uint8 VaultScore = 0;
				if (HitResult.ImpactPoint.Z - GetActorLocation().Z >= 110.f) VaultScore += 2;
				if (FVector::DistXY(GetActorLocation(), VaultStart) >= 250.f) VaultScore += 1;
				VaultType = static_cast<EVaultType>(VaultScore);

				TraceStart = HitResult.ImpactPoint + GetActorForwardVector() * 300.f;
				TraceEnd = TraceStart - FVector(0.f, 0.f, 1000.f);

				if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, TraceChannel, QueryParams))
				{
					VaultEnd = HitResult.ImpactPoint;
					if (static_cast<uint8>(VaultType) >= 2)
					{
						VaultEnd = VaultStart;
						VaultStart = VaultStart - GetActorForwardVector() * 90.f - FVector(0.f, 0.f, 135.f);
					}
				}
				else
				{
					VaultEnd = VaultStart;
				}
				return true;
			}
		}
	}
	return false;
}

void APlayerCharacter::SetMovementMode(ECollisionResponse Response, EMovementMode Mode, bool ShouldTestCameraCollision)
{
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, Response);
	GetCharacterMovement()->SetMovementMode(Mode);
	CameraBoom->bDoCollisionTest = ShouldTestCameraCollision;
}

