


#include "AI/Navigation/PatrolRoute.h"


// Sets default values
APatrolRoute::APatrolRoute()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PatrolRoute = CreateDefaultSubobject<USplineComponent>(TEXT("PatrolRoute"));
	PatrolRoute->SetupAttachment(RootComponent);

	PatrolDirection = 1;
}

void APatrolRoute::IncrementPatrolIndex()
{
	CurrentPatrolIndex += PatrolDirection;
	if (CurrentPatrolIndex <= 0 || CurrentPatrolIndex >= PatrolRoute->GetNumberOfSplinePoints() - 1)
	{
		PatrolDirection *= -1;
	}
}


// Called when the game starts or when spawned
void APatrolRoute::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APatrolRoute::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

