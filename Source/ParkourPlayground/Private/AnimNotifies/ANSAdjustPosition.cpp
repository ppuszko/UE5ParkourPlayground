


#include "AnimNotifies/ANSAdjustPosition.h"


void UANSAdjustPosition::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	ElapsedTime = 0.f;
	Duration = TotalDuration;
	Owner = Cast<ACharacterBase>(MeshComp->GetOwner());
	UE_LOG(LogTemp, Warning, TEXT("Total Duration: %f"), Duration);
	if(Owner)
	{
		OwnerLocation = Owner->GetActorLocation();
		TargetActor = Owner->GetTargetActor();
		if (ShouldMoveToTarget && TargetActor != nullptr)
		{
			float DistanceToTarget = (TargetActor->GetActorLocation() - OwnerLocation).Length();
			DistanceToTravel = FMath::Clamp(DistanceToTarget, 0.f, DistanceToTravel);
		}
		DestinedLocation = Owner->GetActorLocation() + Owner->GetActorForwardVector() * DistanceToTravel;
	}
}
void UANSAdjustPosition::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if (Owner != nullptr)
	{
		ElapsedTime += FrameDeltaTime;
		float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.f, 1.f);
		UE_LOG(LogTemp, Warning, TEXT("Alpha: %f"), Alpha);
		Owner->SetActorLocation(FMath::Lerp(OwnerLocation, DestinedLocation, Alpha));
	}
}
