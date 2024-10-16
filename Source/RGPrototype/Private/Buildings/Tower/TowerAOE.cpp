// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildings/Tower/TowerAOE.h"

#include "NiagaraComponent.h"
#include "Abilities/AbilityBase.h"
#include "DamageTypes/SlowDamageType.h"
#include "Enemy/EnemyBase.h"
#include "Kismet/GameplayStatics.h"


ATowerAOE::ATowerAOE()
{
	AoEEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("AoE Effect"));
	AoEEffect->SetupAttachment(GetRootComponent());
}

void ATowerAOE::Fire_Implementation(AActor* Target)
{
	AttackAbility->ExecuteAbility(Cast<AEnemyBase>(Target), this);
}

void ATowerAOE::BeginPlay()
{
	Super::BeginPlay();
	AoEEffect->Activate();
}

