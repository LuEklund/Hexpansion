// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/AbilityBase.h"
#include "AbilityProjectile.generated.h"

/**
 * 
 */
UCLASS()
class RGPROTOTYPE_API UAbilityProjectile : public UAbilityBase
{
	GENERATED_BODY()
public:
	virtual void ExecuteAbility(AEnemyBase* Target, ATower *Tower) override;
	
	//Tower Attack
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	TSubclassOf<AProjectile>	ProjectileClass;

};
