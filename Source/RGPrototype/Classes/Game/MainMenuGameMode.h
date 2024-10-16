// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

class UWidgetComponent;
/**
 * 
 */
UCLASS()
class RGPROTOTYPE_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()


protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY()
	TObjectPtr<UUserWidget>		MenuWidget;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase>	Music;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget>	MenuWidgetClass;
	
};
