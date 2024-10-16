// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/AbilityInstaDamage.h"

#include "DamageTypes/SlowDamageType.h"
#include "Enemy/EnemyBase.h"
#include "Kismet/GameplayStatics.h"

void UAbilityInstaDamage::ExecuteAbility(AEnemyBase* Target, ATower* Tower)
{
	if (!IsValid(Target)) return;
	Super::ExecuteAbility(Target, Tower);
	if (AEnemyBase *Enemy = Cast<AEnemyBase>(Target))
	{
		UClass		*DamageTypeClass = UDamageType::StaticClass();
		UGameplayStatics::ApplyDamage
		(
			Target,
			Damage,
			nullptr,
			Tower,
			DamageTypeClass
		);
	}
}
