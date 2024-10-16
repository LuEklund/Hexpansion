// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "data/BuildingData.h"
#include "GameFramework/PlayerController.h"
#include "RGPlayerController.generated.h"

class ABuildActor;
class ARGGameMode;
class UEnemySpawnSubsystem;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class RGPROTOTYPE_API ARGPlayerController : public APlayerController
{
	GENERATED_BODY()
private:
	UPROPERTY()
	TObjectPtr<UUserWidget>		EscapeMenuWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget>	EscapeMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABuildActor>	BuildingClass;


	TObjectPtr<ARGGameMode>	RGGameMode;
	

	//Input
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputMappingContext>	MappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction>			ActionMove;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction>			ActionEscape;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction>			ActionZoom;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction>			ActionTestKeyP;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction>			ActionTestKeyK;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction>			ActionLeftClick;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction>			ActionRightClick;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction>			ActionSellHotkey;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction>			ActionUpgradeHotkey;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction>			ActionBackToOriginkey;
	
	void	Move(const FInputActionValue &InputActionValue);
	void	Escape(const FInputActionValue &InputActionValue);
	void	Zoom(const FInputActionValue &InputActionValue);
	void	Spawn(const FInputActionValue &InputActionValue);
	void	KillAll(const FInputActionValue &InputActionValue);
	void	RightClick(const FInputActionValue& InputActionValue);
	void	LeftClick(const FInputActionValue& InputActionValue);
	void	SellHotkey(const FInputActionValue& InputActionValue);
	void	UpgradeHotkey(const FInputActionValue& InputActionValue);
	void	BackToOriginkey(const FInputActionValue& InputActionValue);


	void	InGameUpgarde(const FInputActionValue& InputActionValue);

	UFUNCTION(BlueprintCallable)
	void HideBuildActor();
	void	CursorTrace();
	FHitResult	CursorHit;


public:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<ABuildActor>			BuildingHighlight;
	
	ARGPlayerController();

	UPROPERTY(EditDefaultsOnly)
	float ZoomSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly)
	float CameraMaxZoom = 30000.f;

	UPROPERTY(EditDefaultsOnly)
	float CameraMinZoom = -600.f;

	UPROPERTY(EditDefaultsOnly)
	float MoveSpeed = 100.f;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag 	BuildType;

	UFUNCTION(BlueprintCallable)
	void SetBuildType(FGameplayTag Type);

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

public:
	virtual void PlayerTick(float DeltaTime) override;
};
