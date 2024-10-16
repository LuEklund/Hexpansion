// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InGameUpgrades/InGameUpgradeBase.h"
#include "InGameUpgradeEffect.generated.h"

/**
 * 
 */
UCLASS()
class RGPROTOTYPE_API UInGameUpgradeEffect : public UInGameUpgradeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag	TowerTypeTag = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag	EffectTypeTag = FGameplayTag();

	virtual void	Execute(FInGameUpgradeRow *Upgrade) override;

};
