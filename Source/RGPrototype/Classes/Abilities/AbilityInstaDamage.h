// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/AbilityBase.h"
#include "AbilityInstaDamage.generated.h"

/**
 * 
 */
UCLASS()
class RGPROTOTYPE_API UAbilityInstaDamage : public UAbilityBase
{
	GENERATED_BODY()
	virtual void ExecuteAbility(AEnemyBase* Target, ATower* Tower) override;
};
