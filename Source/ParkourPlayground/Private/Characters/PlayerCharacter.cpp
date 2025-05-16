


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
#include "ActorComponents/AttackComponent.h"
#include "Widgets/PlayerHUDWidget.h"
#include "Widgets/StatsWidgetBase.h"
#include "Components/SphereComponent.h"
#include "GameplayTagAssetInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Characters/EnemyCharacterBase.h"
#include "MotionWarpingComponent.h"



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
	CanMove = true;
	IsRolling = false;
	IsTargetLocked = false;
	bUseControllerRotationYaw = false;
	HasVaulted = false;

	MaxStamina = 150.f;
	CurrentStamina = MaxStamina;

	StaminaDrainRate = 3.f;
	StaminaRegenRate = 6.f;
	StaminaAttackCost = 12.f;

	//SetActorTickInterval(.03f);
}

void APlayerCharacter::ToggleInvincibility(bool Invincible)
{
	Stats->ToggleInvincibility(Invincible);
	IsRolling = Invincible;
	UE_LOG(LogTemp, Warning, TEXT("Invincibility toggled"));
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

	if (AttackComponent)
	{
		AttackComponent->OnAttackFinished.AddUniqueDynamic(this, &APlayerCharacter::OnAttackFinished);
		AttackComponent->OnAttackStarted.AddUniqueDynamic(this, &APlayerCharacter::OnAttackStarted);
	}

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &APlayerCharacter::OnVaultEnded);
	}
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
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CalculateFocusCandidate();

	if (IsTargetLocked)
	{
		if (FocusedObject)
		{

			FVector LookAtStart = GetFollowCamera()->GetComponentLocation();
			LookAtStart -= GetActorForwardVector() * 200.f;
			FVector LookAtEnd = FocusedObject->GetActorLocation();

			Controller->SetControlRotation(UKismetMathLibrary::FindLookAtRotation(LookAtStart, LookAtEnd));
		}
		else
		{
			GetCharacterMovement()->bOrientRotationToMovement = true;
			bUseControllerRotationYaw = false;

			IsTargetLocked = false;
		}
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
		IsSprinting = false;
		CanMove = false;

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

	if (!IsRolling)
	{
		IsRolling = true;
		if (IsTargetLocked)
		{
			UCharacterMovementComponent* CharMovement = GetCharacterMovement();

			FVector CurrVelocity = CharMovement->Velocity;
			CurrVelocity.Z = 0;

			if (!CurrVelocity.IsNearlyZero())
			{
				FVector MoveDir = CurrVelocity.GetSafeNormal();
				FRotator NewRot = MoveDir.Rotation();
			}
		}

		if (RollMontage != nullptr)
		{
			GetMesh()->GetAnimInstance()->Montage_Play(RollMontage);
		}

		UE_LOG(LogTemp, Warning, TEXT("Roll pressed"));
	}


}

void APlayerCharacter::JumpStart(const FInputActionValue& Value)
{
	if (!HasVaulted && IsNearObstacle())
	{
		if (VaultMontages.Contains(VaultType))
		{
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

	UE_LOG(LogTemp, Warning, TEXT("Entered target lock action"));

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

			GetController()->SetControlRotation(UKismetMathLibrary::FindLookAtRotation(
				LookAtStart, FocusedObject->GetActorLocation()));
		}
	}
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

	if (CurrentStamina <= 0.f)
	{
		CurrentStamina = 0.f;
		GetWorld()->GetTimerManager().ClearTimer(StaminaTimer);
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
	CurrentStamina -= StaminaAttackCost;
	UpdateStamina();
}

void APlayerCharacter::OnAttackFinished()
{
	InitStaminaRegen();
	CanMove = true;
}

void APlayerCharacter::OnDetectionRadiusBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AEnemyCharacterBase* Enemy = Cast<AEnemyCharacterBase>(OtherActor))
	{
		IGameplayTagAssetInterface* EnemyTags = Cast<IGameplayTagAssetInterface>(Enemy);

		if (EnemyTags && !EnemyTags->HasMatchingGameplayTag(TeamTags.First()) && !EnemiesInRange.Contains(Enemy))
		{
			EnemiesInRange.Add(Enemy);
			Enemy->ManageHUDDisplay(true);
		}
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
		}
		EnemiesInRange.Remove(Enemy);
		Enemy->ManageHUDDisplay(false);
	}
}

void APlayerCharacter::OnVaultEnded(UAnimMontage* Montage, bool bIsInterrupted)
{
	if (VaultMontages.Contains(VaultType))
	{
		SetMovementMode();
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
			DrawDebugLine(GetWorld(), TraceStart, TraceEnd, HitResult.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 3.0f);
			VaultStart = HitResult.ImpactPoint;
			break;
		}
		else
		{
			DrawDebugLine(GetWorld(), TraceStart, TraceEnd, HitResult.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 3.0f);
		}
	}

	if (FoundHit)
	{
		TraceEnd = HitResult.ImpactPoint + GetActorForwardVector() * 5.f;
		TraceStart = TraceEnd + FVector(0.f, 0.f, 400.f);

		FoundHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, TraceChannel, QueryParams);

		if (FoundHit)
		{
			if (HitResult.ImpactPoint.Z - GetActorLocation().Z >= 250.f)
			{
				return false;
			}
			else
			{
				VaultStart = HitResult.ImpactPoint;
				uint8 VaultScore = 0;
				if (HitResult.ImpactPoint.Z - GetActorLocation().Z >= 110.f) VaultScore += 2;
				if (FVector::DistXY(GetActorLocation(), VaultStart) >= 200.f) VaultScore += 1;
				VaultType = static_cast<EVaultType>(VaultScore);

				TraceStart = HitResult.ImpactPoint + GetActorForwardVector() * 300.f;
				TraceEnd = TraceStart - FVector(0.f, 0.f, 1000.f);

				if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, TraceChannel, QueryParams))
				{
					DrawDebugLine(GetWorld(), TraceStart, TraceEnd, HitResult.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 10.0f);
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

