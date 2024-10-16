// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "data/BuildingData.h"
#include "GameFramework/Pawn.h"
#include "Tower.generated.h"


class ABuildableMapTile;
class UAbilityBase;
class AProjectile;
class ARogueAIController;
class UBehaviorTree;

UCLASS()
class RGPROTOTYPE_API ATower : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATower();
	UPROPERTY(BlueprintReadWrite)
	FBuilding					BuildingInfo;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent>	TowerBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent>	TowerTop;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase>	AttackSound;

	UPROPERTY()
	TObjectPtr<ABuildableMapTile> OwnerTile;
	
	UPROPERTY()
	TObjectPtr<UAbilityBase>	AttackAbility;
	
	void	SetTowerAttackSpeed(float InAttackSpeed);
	float	GetTowerAttackSpeed();
	
	UFUNCTION(BlueprintCallable)
	bool UpgradeSelf();

	UFUNCTION(BlueprintCallable)
	void	Sell();

	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;



	UFUNCTION()
	void	Fire(AActor *Target);
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="AI")
	TObjectPtr<UBehaviorTree>	BehaviorTree;
	
	UPROPERTY()
	TObjectPtr<ARogueAIController>	RogueAIController;
};
