// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RGHUD.h"

#include "Game/RGGameMode.h"
#include "UI/InGameUpgradeWidgetController.h"
#include "UI/OverlayWidgetController.h"
#include "UI/RGWidgetController.h"
#include "UI/Widget/RGUserWidget.h"

UOverlayWidgetController* ARGHUD::GetOverlayWidgetController(const FWidgetControllerParams& Params)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(Params);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return (OverlayWidgetController);
}

UInGameUpgradeWidgetController* ARGHUD::GetInGameUpgardesWidgetController(const FWidgetControllerParams& Params)
{
	if (InGameUpgradeWidgetController == nullptr)
	{
		InGameUpgradeWidgetController = NewObject<UInGameUpgradeWidgetController>(this, InGameUpgradeWidgetControllerClass);
		InGameUpgradeWidgetController->SetWidgetControllerParams(Params);
		InGameUpgradeWidgetController->BindCallbacksToDependencies();
	}
	return (InGameUpgradeWidgetController);
}

void ARGHUD::InitOverlay(ARGPlayerController* PC, ARGGameMode *GM)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitalized, please fil out BP_RGHUD"));

	UUserWidget *Widget =  CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<URGUserWidget>(Widget);
	
	const FWidgetControllerParams WidgetControllerParams(PC, GM);
	UOverlayWidgetController	*WidgetController = GetOverlayWidgetController(WidgetControllerParams);
	
	OverlayWidget->SetWidgetController(WidgetController);
	// WidgetController->BroadcastInitialValues();
	
	Widget->AddToViewport();
}
