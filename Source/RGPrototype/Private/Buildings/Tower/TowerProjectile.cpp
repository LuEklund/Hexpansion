// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildings/Tower/TowerProjectile.h"

#include "Abilities/AbilityBase.h"
#include "Enemy/EnemyBase.h"

void ATowerProjectile::Fire_Implementation(AActor* Target)
{
	// UE_LOG(LogTemp, Warning, TEXT("Tower fire called in C++"));
	DoFire(Target);
}


void ATowerProjectile::DoFire(AActor* Target)
{
	// UE_LOG(LogTemp, Warning, TEXT("Attacking in C++"));
	AttackAbility->ExecuteAbility(Cast<AEnemyBase>(Target), this);
}
