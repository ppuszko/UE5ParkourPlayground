


#include "AnimNotifies/ANSpawnAOEDamage.h"
#include "Characters/CharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/Damageable.h"




void UANSpawnAOEDamage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	UE_LOG(LogTemp, Warning, TEXT("Notify Triggered"))
	if (ACharacterBase* AttackInstigator = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Has instigator"))
		UAttackComponent* AttackComp = AttackInstigator->GetAttackComponent();
		if (AttackComp)
		{
			UE_LOG(LogTemp, Warning, TEXT("Has Attack comp"))
			if (const UAttackDataAsset* SpecialAttack = AttackComp->GetCurrentSpecialAttack())
			{
				UE_LOG(LogTemp, Warning, TEXT("Has attack data"))
				TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
				ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

				TArray<AActor*> ActorsToIgnore;
				ActorsToIgnore.Add(AttackInstigator);

				TArray<AActor*> HitActors;

				FVector SphereLocation = AttackInstigator->GetActorLocation() + AttackInstigator->GetActorForwardVector() * 300.f;
				UKismetSystemLibrary::SphereOverlapActors(
					AttackInstigator->GetWorld(),
					SphereLocation,
					SpecialAttack->SpecialAreaOfEffect,
					ObjectTypes, ACharacterBase::StaticClass(),
					ActorsToIgnore,
					HitActors);

				//DrawDebugSphere(
				//	AttackInstigator->GetWorld(),
				//	AttackInstigator->GetActorLocation() + AttackInstigator->GetActorForwardVector() * 300.f,
				//	SpecialAttack->SpecialAreaOfEffect,
				//	16,
				//	FColor(0, 255, 255, 255),
				//	false,
				//	3.f);

				for (AActor* HitActor : HitActors)
				{
					UE_LOG(LogTemp, Warning, TEXT("Has overlapped with actor"))
					if (IDamageable* ActorToDamage = Cast<IDamageable>(HitActor))
					{
						UE_LOG(LogTemp, Warning, TEXT("Actor implements interface"))
						if (ActorToDamage->ShouldResolveAttackCollision(AttackInstigator))
						{
							UE_LOG(LogTemp, Warning, TEXT("Actor should be damaged"))
							FVector HitDirection;
							bool HitReactRight = ActorToDamage->CalculateHitDirection(AttackInstigator->GetActorLocation() + AttackInstigator->GetActorForwardVector() * 300.f, HitDirection);

							ActorToDamage->TakeDamage(AttackInstigator, HitReactRight);
						}
					}

				}
			}
		}
	}
	

}