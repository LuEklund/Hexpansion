// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/RGBlueprintFunctionLibrary.h"

#include "Game/RGGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "UI/RGHUD.h"
#include "UI/RGWidgetController.h"


bool URGBlueprintFunctionLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams,
	ARGHUD*& OutRPGHUD)
{
	if (APlayerController *PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutRPGHUD = Cast<ARGHUD>(PC->GetHUD());
		if (OutRPGHUD)
		{
			OutWCParams.PlayerController = PC;
			OutWCParams.GameMode =PC->GetWorld()->GetAuthGameMode<ARGGameMode>();
			return (true);
		}
	}
	return false;
}

UOverlayWidgetController* URGBlueprintFunctionLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ARGHUD	*RPGHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, RPGHUD))
	{
		return (RPGHUD->GetOverlayWidgetController(WCParams));
	}
	return (nullptr);
}

UInGameUpgradeWidgetController* URGBlueprintFunctionLibrary::GetInGameUpgradeWidgetController(
	const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ARGHUD	*RPGHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, RPGHUD))
	{
		return (RPGHUD->GetInGameUpgardesWidgetController(WCParams));

	}
	return (nullptr);
}

TArray<FRotator> URGBlueprintFunctionLibrary::EvenlySpacedRotators(const FVector& Forward, const FVector& Axis,
	float Spread, int32 NumRotators)
{
	TArray<FRotator>	Rotators;
	
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	if (NumRotators > 1)
	{
		const float DeltaSpread = Spread / (NumRotators - 1);
		for (int32 i = 0; i < NumRotators; ++i)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Rotators.Add(Direction.Rotation());
		}
	}
	else
	{
		Rotators.Add(Forward.Rotation());
	}
	return Rotators;
}

TArray<FVector> URGBlueprintFunctionLibrary::EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis,
	float Spread, int32 NumVectors)
{
	TArray<FVector>	Rotators;
	
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	if (NumVectors > 1)
	{
		const float DeltaSpread = Spread / (NumVectors - 1);
		for (int32 i = 0; i < NumVectors; ++i)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Rotators.Add(Direction);
		}
	}
	else
	{
		Rotators.Add(Forward);
	}
	return Rotators;
}
