// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RGWidgetController.h"

#include "Game/RGGameMode.h"
#include "Player/RGPlayerController.h"

ARGPlayerController* URGWidgetController::GetRG_PC()
{
	if (RGPlayerController == nullptr)
	{
		RGPlayerController = Cast<ARGPlayerController>(PlayerController);
	}
	return RGPlayerController;
}

ARGGameMode* URGWidgetController::GetRG_GM()
{
	if (RGGameMode == nullptr)
	{
		RGGameMode = Cast<ARGGameMode>(GameMode);
	}
	return RGGameMode;
}

void URGWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	GameMode = WCParams.GameMode;
}
