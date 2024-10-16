// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/AbilityBase.h"

#include "Enemy/EnemyBase.h"
#include "Game/RG_GameInstance.h"

void UAbilityBase::ExecuteAbility(AEnemyBase* Target, ATower *Tower)

{
	Damage = Tower->BuildingInfo.AttackDamage * GetWorld()->GetGameInstance<URG_GameInstance>()->DifficultyScale;
	// UE_LOG(LogTemp, Error, TEXT("Difficulty: %f, DAMAGE %f"),  GetWorld()->GetGameInstance<URG_GameInstance>()->DifficultyScale, Damage);

	Target->PreDamage += Damage;
	
}
