// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RGWidgetController.h"
#include "GameFramework/HUD.h"
#include "Player/RGPlayerController.h"
#include "RGHUD.generated.h"

class UInGameUpgradeWidgetController;
class UOverlayWidgetController;
class URGUserWidget;
/**
 * 
 */
UCLASS()
class RGPROTOTYPE_API ARGHUD : public AHUD
{
	GENERATED_BODY()
public:
	void InitOverlay(ARGPlayerController* PC, ARGGameMode* GM);
	
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& Params);
	UInGameUpgradeWidgetController* GetInGameUpgardesWidgetController(const FWidgetControllerParams& Params);

private:
	UPROPERTY()
	TObjectPtr<URGUserWidget>	OverlayWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<URGUserWidget>	OverlayWidgetClass;
	UPROPERTY()
	TObjectPtr<URGUserWidget>	InGameUpgradeWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<URGUserWidget>	InGameUpgradeWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController>	OverlayWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController>	OverlayWidgetControllerClass;
	UPROPERTY()
	TObjectPtr<UInGameUpgradeWidgetController>	InGameUpgradeWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UInGameUpgradeWidgetController>	InGameUpgradeWidgetControllerClass;

};

