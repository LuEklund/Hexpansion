// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InGameUpgrades/InGameUpgradeBase.h"
#include "InGameUpgradeTower.generated.h"

/**
 * 
 */
UCLASS()
class RGPROTOTYPE_API UInGameUpgradeTower : public UInGameUpgradeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag	BuildingType = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag	UpgradeType = FGameplayTag();
	

	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// TObjectPtr<const UTexture2D>	Icon = nullptr;
	

	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// float	IncreaseFlatDamage = 0;

	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// bool bUsed = false;
	virtual void	Execute(FInGameUpgradeRow *Upgrade) override;

};


