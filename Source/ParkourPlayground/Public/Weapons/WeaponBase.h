

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/Data/WeaponDataAsset.h"

#include "WeaponBase.generated.h"

class UStaticMeshComponent;
class UCapsuleComponent;
class ACharacterBase;

UCLASS(Abstract, Blueprintable)
class PARKOURPLAYGROUND_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	UFUNCTION(BlueprintCallable)
	void ToggleActive();

	UFUNCTION(BlueprintCallable)
	const UAttackDataAsset*  GetAttackDataByIndex(int Index) const { return WeaponDataAsset->GetAttackData(Index); }

	UFUNCTION(BlueprintCallable)
	int GetAttackCount() const { return WeaponDataAsset->GetAttackCount(); }

	UFUNCTION(BlueprintCallable)
	FSDamageInfo ConstructDamageInfo(int Index);

	UFUNCTION(BlueprintCallable)
	FTransform GetWeaponSocketOffset() const { return WeaponDataAsset->SocketTransform; }

	UFUNCTION(BlueprintCallable)
	void ClearHitActors() { HitActors.Empty(); }

	UFUNCTION(BlueprintCallable)
	UCapsuleComponent* GetCollider() { return CapsuleComponent.Get(); }

protected:
	// Sets default values for this actor's properties
	AWeaponBase();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleOverlapBegin(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

protected:

	//Member Variables
	UPROPERTY(Category=Weapon, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(Category=Weapon, VisibleAnywhere, BlueprintReadonly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	UPROPERTY(Category=Weapon, EditDefaultsOnly, BlueprintReadOnly)
	UWeaponDataAsset* WeaponDataAsset;

	//Helper methods
	bool CanBeAttacked(AActor* TargetActor, AActor* OwningActor);

	//Helper variables
	bool IsActive;
	TArray<AActor*> HitActors;
};
