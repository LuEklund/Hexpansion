// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "ButtonUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class RGPROTOTYPE_API UButtonUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ButtonIcon;

	
};
