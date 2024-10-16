// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "data/BuildingData.h"
#include "UI/RGWidgetController.h"
#include "InGameUpgradeWidgetController.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHidInGameUpgrade);


UCLASS(BlueprintType, Blueprintable)
class RGPROTOTYPE_API UInGameUpgradeWidgetController : public URGWidgetController
{
	GENERATED_BODY()
public:
	virtual void	BindCallbacksToDependencies();
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Stats")
	FOnHidInGameUpgrade	OnHideInGameUpgrade;

	
};
