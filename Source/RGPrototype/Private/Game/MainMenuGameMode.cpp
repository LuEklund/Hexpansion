// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MainMenuGameMode.h"

#include "Components/WidgetComponent.h"
#include "Game/RG_GameInstance.h"
#include "Game/RG_GameState.h"


void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();


	//Create menu widget
	MenuWidget = NewObject<UUserWidget>(this, MenuWidgetClass);
	MenuWidget->AddToViewport();

	//Get player controller
	APlayerController *PC = GetWorld()->GetFirstPlayerController();

	//Setup PC settings
	PC->bShowMouseCursor = true;
	PC->DefaultMouseCursor = EMouseCursor::Default;
	PC->bEnableClickEvents = true;
	PC->bEnableMouseOverEvents = true;

	//Ensure the game mode is UI only
	FInputModeUIOnly  InputModeData;
	InputModeData.SetWidgetToFocus(MenuWidget->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PC->SetInputMode(InputModeData);

	//Disable player look and move
	PC->SetIgnoreLookInput(true);
	PC->SetIgnoreMoveInput(true);

	//Play music
	GetGameInstance<URG_GameInstance>()->PlayMusic(Music);
}
