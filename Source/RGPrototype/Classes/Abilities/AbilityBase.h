// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Buildings/Tower/TowerProjectile.h"
#include "UObject/NoExportTypes.h"
#include "AbilityBase.generated.h"

class AEnemyBase;
/**
 * 
 */
UCLASS(Blueprintable)
class RGPROTOTYPE_API UAbilityBase : public UObject
{
	GENERATED_BODY()
public:
	virtual void ExecuteAbility(AEnemyBase* Target, ATower *Tower);

protected:
	float	Damage = 0.f;
};
