// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/AbilitySlowness.h"

#include "DamageTypes/SlowDamageType.h"
#include "Enemy/EnemyBase.h"
#include "Kismet/GameplayStatics.h"

void UAbilitySlowness::ExecuteAbility(AEnemyBase* Target, ATower* Tower)
{
	if (!IsValid(Target)) return;
	// Super::ExecuteAbility(Target, Tower);

	if (AEnemyBase *Enemy = Cast<AEnemyBase>(Target))
	{
		UClass		*DamageTypeClass = USlowDamageType::StaticClass();
		UGameplayStatics::ApplyDamage
		(
			Target,
			Tower->BuildingInfo.Slow,
			nullptr,
			Tower,
			DamageTypeClass
		);
	}
}
