// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/AbilityProjectile.h"

#include "Actor/Projectile.h"
#include "Enemy/EnemyBase.h"
#include "Game/RGBlueprintFunctionLibrary.h"

void UAbilityProjectile::ExecuteAbility(AEnemyBase* Target, ATower *Tower)
{

	FVector	Direction = Target->GetActorLocation() - Tower->ProjectileSpawnPoint->GetComponentLocation();
	Direction.Normalize();

	TArray<FVector> AngelVectors = URGBlueprintFunctionLibrary::EvenlyRotatedVectors(Direction, FVector::UpVector, 180.f, 1);

	for (FVector AngelVector : AngelVectors)
	{
		Super::ExecuteAbility(Target, Tower);
		FRotator ProjectileRotation = AngelVector.Rotation();
		// DrawDebugDirectionalArrow(GetWorld(), AngelVector + Tower->ProjectileSpawnPoint->GetComponentLocation(), Tower->ProjectileSpawnPoint->GetComponentLocation() + AngelVector * 100.f, 10.f, FColor::Red, false, 2.f);
		AProjectile	*Projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileClass,
		Tower->ProjectileSpawnPoint->GetComponentLocation() + AngelVector * 100.f,
		ProjectileRotation);
		
		// Projectile->ProjectileMovement->HomingTargetComponent = Target;
		
		AEnemyBase	*Enemy = Cast<AEnemyBase>(Target);
		
		Projectile->Target = Enemy;
		Projectile->Damage = Damage;
		Projectile->Slow = Tower->BuildingInfo.Slow;
		Projectile->Bleed = Tower->BuildingInfo.Bleed;
		Projectile->Stun = Tower->BuildingInfo.Stun;
		
		Projectile->SetOwner(Tower);
	}
}
