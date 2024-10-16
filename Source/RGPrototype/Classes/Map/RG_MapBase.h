// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RG_MapBase.generated.h"

class USplineComponent;

UCLASS(Abstract)
class RGPROTOTYPE_API ARG_MapBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARG_MapBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void Expand();
	virtual void Expand(FVector ExpandPoint);
	virtual void GetPositions(std::vector<FVector> &InVector);
	virtual TArray<USplineComponent *> GetSplines();
	UFUNCTION(BlueprintCallable)
	virtual FVector GetEndNode();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual USplineComponent* GetSpline();



};
