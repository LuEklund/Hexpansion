// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Buildings/Tower.h"
#include "TowerProjectile.generated.h"

/**
 * 
 */
UCLASS()
class RGPROTOTYPE_API ATowerProjectile : public ATower
{
	GENERATED_BODY()
public:
	void Fire_Implementation(AActor* Target);

	UFUNCTION(BlueprintCallable)
	void DoFire(AActor* Target);
};
