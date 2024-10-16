// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InGameUpgradeInterface.generated.h"

struct  FInGameUpgradeRow;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInGameUpgradeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RGPROTOTYPE_API IInGameUpgradeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual int32	GetXPCost() = 0;
	virtual void	Execute(FInGameUpgradeRow *Upgrade) = 0;
};
