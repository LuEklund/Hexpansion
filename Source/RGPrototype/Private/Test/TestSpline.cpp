// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TestSpline.h"

#include "Components/SplineComponent.h"

// Sets default values
ATestSpline::ATestSpline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineMain = CreateDefaultSubobject<USplineComponent>("SplineMain");
	SplineMain->SetupAttachment(GetRootComponent());
	SplineSplit1 = CreateDefaultSubobject<USplineComponent>("SplineSplit1");
	SplineSplit1->SetupAttachment(GetRootComponent());
	SplineSplit2 = CreateDefaultSubobject<USplineComponent>("SplineSplit2");
	SplineSplit2->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void ATestSpline::BeginPlay()
{
	Super::BeginPlay();

	SplineMain->ClearSplinePoints();
	SplineSplit1->ClearSplinePoints();
	SplineSplit2->ClearSplinePoints();

	// Add points to SplineMain
	for (int32 i = 0; i < 3; ++i)
	{
		FVector PointLocation(i * 100.f, 0.f, 0.f);
		SplineMain->AddSplinePoint(PointLocation, ESplineCoordinateSpace::Local);
	}

	// Ensure the spline is updated to calculate the last point correctly
	SplineMain->UpdateSpline();

	// Get the last point location of SplineMain
	int32 LastPointIndex = SplineMain->GetNumberOfSplinePoints() - 1;
	FVector LastPointLocation = SplineMain->GetLocationAtSplinePoint(LastPointIndex, ESplineCoordinateSpace::Local);

	// Attach SplineSplit1 and SplineSplit2 to SplineMain
	SplineSplit1->AttachToComponent(SplineMain, FAttachmentTransformRules::KeepWorldTransform);
	SplineSplit2->AttachToComponent(SplineMain, FAttachmentTransformRules::KeepWorldTransform);

	// Set the relative location of SplineSplit1 and SplineSplit2 to the last point of SplineMain
	SplineSplit1->SetRelativeLocation(LastPointLocation);
	SplineSplit2->SetRelativeLocation(LastPointLocation);

	// Add additional points to SplineSplit1
	for (int32 i = 1; i <= 3; ++i)
	{
		FVector PointLocation = FVector(0.f, i * 100.f, 0.f); // Continue from the last point
		SplineSplit1->AddSplinePoint(PointLocation, ESplineCoordinateSpace::Local);
	}

	// Add additional points to SplineSplit2
	for (int32 i = 1; i <= 3; ++i)
	{
		FVector PointLocation = FVector(0.f, i * -100.f, 0.f); // Continue from the last point
		SplineSplit2->AddSplinePoint(PointLocation, ESplineCoordinateSpace::Local);
	}

	// Optionally update the new splines
	SplineSplit1->UpdateSpline();
	SplineSplit2->UpdateSpline();
	
}

// Called every frame
void ATestSpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

