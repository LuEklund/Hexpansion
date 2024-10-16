// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "RGAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class RGPROTOTYPE_API URGAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	static URGAssetManager &Get();
protected:
	virtual void	StartInitialLoading() override;
};
