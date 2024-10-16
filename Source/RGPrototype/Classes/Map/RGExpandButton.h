// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RGPrototype/Classes/Game/RGGameMode.h"
#include "RGExpandButton.generated.h"

class UWidgetComponent;

UCLASS()
class RGPROTOTYPE_API ARGExpandButton : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	EUpgradeType MyType;
	
	void SetMyType(EUpgradeType Type);
	// Sets default values for this actor's properties
	ARGExpandButton();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* RootSceneComponent;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UWidgetComponent>		ButtonWidgetComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent>		InfoWidgetComponent;
	
	// UPROPERTY(EditDefaultsOnly)
	// TObjectPtr<UStaticMeshComponent>	Mesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
