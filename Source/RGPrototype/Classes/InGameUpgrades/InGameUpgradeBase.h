// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Interface/InGameUpgradeInterface.h"
#include "InGameUpgradeBase.generated.h"
/**
 * 
 */
UCLASS(Blueprintable)
class RGPROTOTYPE_API UInGameUpgradeBase :  public UObject, public IInGameUpgradeInterface
{
	GENERATED_BODY()

public:
	
	virtual int32	GetXPCost() override;
	virtual void	Execute(FInGameUpgradeRow *Upgrade) override;
	
protected:
	float IncreaseValue = 0.f;
};

