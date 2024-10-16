// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestSpline.generated.h"

class USplineComponent;

UCLASS()
class RGPROTOTYPE_API ATestSpline : public AActor
{
	GENERATED_BODY()
	
public:	
	ATestSpline();

	//Path for enemies
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent>	SplineMain;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent>	SplineSplit1;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent>	SplineSplit2;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
