// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class AEnemyBase;
class UNiagaraComponent;

UCLASS()
class RGPROTOTYPE_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent>	ProjectileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Props")
	float	Speed = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Props")
	float	ProximityImpact = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Props")
	float	Damage = 50.f;

	float Bleed = 0.f;
	float Stun = 0.f;
	float Slow = 0.f;

	UPROPERTY()
	AEnemyBase	*Target;

	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TObjectPtr<UNiagaraComponent>	TrailParticles;

	// UPROPERTY(EditAnywhere, Category = "Combat")
	// TObjectPtr<USoundBase>	LaunchSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase *HitSound;

protected:
	virtual void BeginPlay() override;
	

};
