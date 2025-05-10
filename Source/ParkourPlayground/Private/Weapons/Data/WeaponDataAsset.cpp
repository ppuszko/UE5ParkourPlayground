


#include "Weapons/Data/WeaponDataAsset.h"

const UAttackDataAsset* UWeaponDataAsset::GetAttackData(int Index) const
{
    if (Index > 0 && Index < AttackStyle.Num())
    {
        return AttackStyle[Index];
    }
    return nullptr;
}
