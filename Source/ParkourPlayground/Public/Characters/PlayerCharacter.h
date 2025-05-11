

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class PARKOURPLAYGROUND_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LightAttackAction;

public:
	APlayerCharacter();

protected:

	virtual void BeginPlay() override;

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	virtual void Move(const FInputActionValue& Value);

	virtual void Look(const FInputActionValue& Value);

	virtual void LightAttack(const FInputActionValue& Value);

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
