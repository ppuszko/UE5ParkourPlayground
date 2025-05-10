

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class UStaticMeshComponent;
class UCapsuleComponent;

UCLASS(Abstract, Blueprintable)
class PARKOURPLAYGROUND_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	UFUNCTION(BlueprintCallable)
	void ToggleActive();

protected:
	// Sets default values for this actor's properties
	AWeaponBase();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleOverlapBegin(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);


	//Member Variables
	UPROPERTY(Category=Weapon, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(Category=Weapon, VisibleAnywhere, BlueprintReadonly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	//helper variables
	bool IsActive = false;
	TArray<AActor*> HitActors;
};
