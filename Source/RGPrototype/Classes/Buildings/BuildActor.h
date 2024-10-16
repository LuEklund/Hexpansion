// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/RGPlayerController.h"
#include "BuildActor.generated.h"

class UWidgetComponent;

UCLASS()
class RGPROTOTYPE_API ABuildActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildActor();
	
	TObjectPtr<ARGGameMode>  RGGameMode;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent>	MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDecalComponent>	AttackRangeCircle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UWidgetComponent>	TextWidget;
	
	//Colors for the actor
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance>	Available;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance>	Unavailable;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	UFUNCTION(BlueprintCallable)
	void SetDisplayAttackRange(float AttackRange);
	void Update(const FHitResult& HitResult, FGameplayTag BuildType);

	UFUNCTION(BlueprintImplementableEvent, Category = "YourCategory")
	void SetExtraHeigthText(const FText& Text);
	
	// UFUNCTION(BlueprintImplementableEvent)
	// void	SetExtraHeigthText(FText Text);
};
