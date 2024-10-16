// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/RGHUD.h"
#include "UI/RGWidgetController.h"
#include "RGBlueprintFunctionLibrary.generated.h"

class UOverlayWidgetController;
class UInGameUpgradeWidgetController;
/**
 * 
 */
UCLASS()
class RGPROTOTYPE_API URGBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category="BlueprintLibrary|WidgetController")
	static bool MakeWidgetControllerParams(const UObject* Object, FWidgetControllerParams& WcParams, ARGHUD*& Arghud);
	
	UFUNCTION(BlueprintPure, Category="BlueprintLibrary|WidgetController",  meta=(DefaultToSelf="WorldContextObject"))
	static UOverlayWidgetController	*GetOverlayWidgetController(const UObject *WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category="BlueprintLibrary|WidgetController",  meta=(DefaultToSelf="WorldContextObject"))
	static UInGameUpgradeWidgetController	*GetInGameUpgradeWidgetController(const UObject *WorldContextObject);


	//=======================
	// Mechanics
	//=======================
	UFUNCTION(BlueprintPure, Category="BlueprintLibrary|GameplayMechanics")
	static TArray<FRotator>	EvenlySpacedRotators(const FVector& Forward, const FVector &Axis, float Spread, int32 NumRotators);

	UFUNCTION(BlueprintPure, Category="BlueprintLibrary|GameplayMechanics")
	static TArray<FVector>	EvenlyRotatedVectors(const FVector& Forward, const FVector &Axis, float Spread, int32 NumVectors);
	

};
