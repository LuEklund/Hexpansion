// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UI/OverlayWidgetController.h"
#include "EnemyBase.generated.h"

class USplineComponent;
class UWidgetComponent;
class ARGGameMode;


UCLASS(Abstract)
class RGPROTOTYPE_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemyBase();

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USplineComponent> 	Spline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float	GoldValue = 10;

	//Slowed
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool	bIsSlowed = false;
	
	//BLEED
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float	BleedDamage = 0.f;

	//STUNNED
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool	bIsStunned = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float	DistanceTraveled = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float	DistanceToReachBase = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float	SplineOffset = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Health")
	float	Health = 100.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Health")
	float	HealthMultiplier = 1.f;
	
	UPROPERTY(BlueprintReadOnly)
	float	MaxHealth = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float	PreDamage = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float	ActorBaseSpeed = 400.f;
	
	UPROPERTY(BlueprintReadWrite)
	float	ActorSpeed = 400.f;

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnValueChange	OnHealthChangeDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Stun")
	FOnValueChange	OnUnstunDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Slow")
	FOnValueChange	OnSlowDelegate;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void	HandleDeath();
	void	SetUpEnemy(USplineComponent* InSpline);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Gear")
	TObjectPtr<USkeletalMeshComponent>	Helmet;
	
	UPROPERTY(EditAnywhere, Category="Gear")
	FName HelmetSocketName;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent>	HealthBar;

private:

	TObjectPtr<ARGGameMode>	GameMode;

	FTimerHandle	SlowTimerHandle;
	FTimerHandle	BleedTimerHandle;
	FTimerHandle	StunTimerHandle;



	UFUNCTION()
	void	HandleBleed();
	void ApplySlow(float Damage);

	UFUNCTION()
	void	DamageTaken(AActor *DamagedActor, float Damage, const UDamageType *DamageType, class AController *Instgator, AActor *DamageCauser);


};
