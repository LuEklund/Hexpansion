// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class RGPROTOTYPE_API APlayerPawn : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Base")
	TObjectPtr<UStaticMeshComponent>	BackgroundPlane;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Base")
	TObjectPtr<UStaticMeshComponent>	BackgroundPlaneBackground;
	
	UPROPERTY(EditDefaultsOnly, Category="Base")
	TObjectPtr<USpringArmComponent>	SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, Category="Base")
	TObjectPtr<UCameraComponent>	CameraComponent;
	
	APlayerPawn();
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintImplementableEvent)
	void	BackgroundPlaneOpacity(float Value);

protected:
	virtual void BeginPlay() override;

		
	
};
