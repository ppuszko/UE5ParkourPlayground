


#include "AI/Controllers/AIControllerBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"

AAIControllerBase::AAIControllerBase()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	SightConfig->SightRadius = 2000.f;
	SightConfig->LoseSightRadius = 2200.f;
	SightConfig->PeripheralVisionAngleDegrees = 90.f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = false;

	AIPerception->ConfigureSense(*SightConfig);
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());

	LineOfSightKey = TEXT("HasLineOfSight");
	TargetActorKey = TEXT("TargetActor");

	bAttachToPawn = true;
	SetMoveBlockDetection(true);


}

void AAIControllerBase::OnSightPerceptionUpdated(AActor* SourceActor, FAIStimulus Stimulus)
{
	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			GetWorld()->GetTimerManager().ClearTimer(LineOfSightTimer);
			GetBlackboardComponent()->SetValueAsBool(LineOfSightKey, true);
			GetBlackboardComponent()->SetValueAsObject(TargetActorKey, SourceActor);
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(LineOfSightTimer, this, &AAIControllerBase::OnTimer, 1.f, true);
		}
	}
}

void AAIControllerBase::OnTimer()
{
	TimeElapsed += 1.f;

	if (TimeElapsed >= ChaseTimeAfterLostSight)
	{
		GetWorld()->GetTimerManager().ClearTimer(LineOfSightTimer);
		TimeElapsed = 0.f;
		GetBlackboardComponent()->SetValueAsBool(LineOfSightKey, false);
		GetBlackboardComponent()->SetValueAsObject(TargetActorKey, nullptr);
	}
}

ETeamAttitude::Type AAIControllerBase::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IGenericTeamAgentInterface* OtherAgent = Cast<const IGenericTeamAgentInterface>(&Other);
	const IGenericTeamAgentInterface* ThisAgent = Cast<const IGenericTeamAgentInterface>(GetPawn());
	if (OtherAgent && ThisAgent)
	{
		return OtherAgent->GetGenericTeamId() == ThisAgent->GetGenericTeamId() ? ETeamAttitude::Friendly : ETeamAttitude::Hostile;
	}
	return ETeamAttitude::Neutral;
}

void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay();

	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AAIControllerBase::OnSightPerceptionUpdated);

}

void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	FTimerHandle InitHandle;

	GetWorld()->GetTimerManager().SetTimer(InitHandle,
		[this](){
		UE_LOG(LogTemp, Warning, TEXT("Running BT after delay"));
		RunBehaviorTree(BehaviorTree);
		BrainComponent->RestartLogic(); },
		.2f,
		false);

}