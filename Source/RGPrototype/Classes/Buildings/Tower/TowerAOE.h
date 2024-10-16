// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Buildings/Tower.h"
#include "TowerAOE.generated.h"

class UNiagaraComponent;
/**
 * 
 */
UCLASS()
class RGPROTOTYPE_API ATowerAOE : public ATower
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TObjectPtr<UNiagaraComponent>	AoEEffect;

	ATowerAOE();

public:
	void Fire_Implementation(AActor* Target);

protected:
	virtual void BeginPlay() override;
};
