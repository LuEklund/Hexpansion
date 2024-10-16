// Fill out your copyright notice in the Description page of Project Settings.


#include "RGAssetManager.h"

#include "RGGameplayTags.h"

URGAssetManager& URGAssetManager::Get()
{
	check(GEngine);
	URGAssetManager *RPGAssetManager = Cast<URGAssetManager>(GEngine->AssetManager);
	return (*RPGAssetManager);
}

void URGAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FRGGameplayTags::InitializeNativeGameplayTags();

	//FOR GAS
	// This is required to use target data. But automatically enabled in Unreal 5.3+
	// UAbilitySystemGlobals::Get().InitGlobalData();
}
