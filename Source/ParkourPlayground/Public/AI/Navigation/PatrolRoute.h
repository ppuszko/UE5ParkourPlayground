

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"

#include "PatrolRoute.generated.h"


UCLASS(Blueprintable)
class PARKOURPLAYGROUND_API APatrolRoute : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolRoute();

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	void IncrementPatrolIndex();
	
	UFUNCTION(BlueprintCallable, Category = "Navigation")
	FVector GetCurrentPatrolDestination(){ return PatrolRoute->GetLocationAtSplinePoint(CurrentPatrolIndex, ESplineCoordinateSpace::World); }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Navigation")
	USplineComponent* PatrolRoute;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Navigation")
	int CurrentPatrolIndex;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Navigation")
	int PatrolDirection;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
