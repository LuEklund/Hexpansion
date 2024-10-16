// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/RG_MapBase.h"

// Sets default values
ARG_MapBase::ARG_MapBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARG_MapBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARG_MapBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARG_MapBase::Expand()
{
	UE_LOG(LogTemp, Warning, TEXT("ERROR: ARG_MapBase::Expand -> Please setup a Expand function call!"));
}

void ARG_MapBase::Expand(FVector ExpandPoint)
{
	UE_LOG(LogTemp, Warning, TEXT("ERROR: ARG_MapBase::Expand(FVector ExpandPoint) -> Please setup a Expand function call!"));
}

void ARG_MapBase::GetPositions(std::vector<FVector>& InVector)
{
	UE_LOG(LogTemp, Warning, TEXT("ERROR: ARG_MapBase::Expand -> Please setup a Expand function call!"));
}

TArray<USplineComponent*> ARG_MapBase::GetSplines()
{
	UE_LOG(LogTemp, Warning, TEXT("ERROR: ATArray<USplineComponent*> ARG_MapBase::GetSplines()-> Please setup a GetSplines function call!"));
	return TArray<USplineComponent*>();
}

FVector ARG_MapBase::GetEndNode()
{
	UE_LOG(LogTemp, Warning, TEXT("ERROR: ARG_MapBase::GetEndNode -> Please setup a EndNode function call!"));
	return FVector::ZeroVector;
}

USplineComponent* ARG_MapBase::GetSpline()
{
	UE_LOG(LogTemp, Warning, TEXT("ERROR: ARG_MapBase::GetSpline -> Please setup a Spline function call!"));
	return nullptr;
}

