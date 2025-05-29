

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DataTypes/DataTypes.h"

#include "Damageable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PARKOURPLAYGROUND_API IDamageable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool TakeDamage(AActor* Causer, bool HitReactRight) = 0;
	virtual void ToggleInvincibility(bool Invincible){}
	virtual bool ShouldResolveAttackCollision(AActor* AttackingActor) = 0;
	// Calculates whether attack came from right or left side.
	// To obtain exact direction, one should use ResultingHitDirection, else it's irrelevant and provided only to avoid overloads
	virtual bool CalculateHitDirection(FVector HitLocation, FVector& ResultingHitDirection) = 0;
};
