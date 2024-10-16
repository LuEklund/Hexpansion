// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RGWidgetController.generated.h"

class AGameMode;
class ARGGameMode;
class ARGPlayerController;
/**
 * 
 */


USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()
	FWidgetControllerParams(){}
	FWidgetControllerParams(APlayerController *PC, AGameMode *GM)
	: PlayerController(PC),
	GameMode(GM) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController>		PlayerController = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AGameMode>				GameMode = nullptr;
};

UCLASS()
class RGPROTOTYPE_API URGWidgetController : public UObject
{
	GENERATED_BODY()
protected:

	//Default Params for controller
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerController>		PlayerController;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<AGameMode>				GameMode;
	
	//RPG Params for controller
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<ARGPlayerController>		RGPlayerController;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
    TObjectPtr<ARGGameMode>				RGGameMode;

	// UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	// TObjectPtr<ARGPlayerState>			RPGPlayerState;

	// UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	// TObjectPtr<URPGAbilitySystemComponent>	RPGAbilitySystemComponent;

	// UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	// TObjectPtr<URPGAttributeSet>			RPGAttributeSet;

	ARGPlayerController	*GetRG_PC();
	ARGGameMode			*GetRG_GM();

public:
	UFUNCTION(BlueprintCallable)
	void	SetWidgetControllerParams(const FWidgetControllerParams &WCParams);
};
