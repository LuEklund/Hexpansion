// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyBase.h"

#include "Actor/Projectile.h"
#include "Buildings/Tower.h"
#include "Components/SplineComponent.h"
#include "Components/WidgetComponent.h"
#include "DamageTypes/BleedDamageType.h"
#include "DamageTypes/SlowDamageType.h"
#include "Game/RGGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Widget/RGUserWidget.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Helmet = CreateDefaultSubobject<USkeletalMeshComponent>("HelmetMesh");
	Helmet->SetupAttachment(GetMesh(), HelmetSocketName);
	Helmet->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	MaxHealth = Health * HealthMultiplier;
	OnTakeAnyDamage.AddDynamic(this, &AEnemyBase::DamageTaken);
	
	SplineOffset = std::rand() % 35;
	if (std::rand() % 2 == 0)
		SplineOffset *= -1;
	
	GameMode = GetWorld()->GetAuthGameMode<ARGGameMode>();
	
	if (URGUserWidget *RPGUserWidget = Cast<URGUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		RPGUserWidget->SetWidgetController(this);
		
	}
	
	GetWorldTimerManager().SetTimer(BleedTimerHandle, this, &AEnemyBase::HandleBleed, .1f, true);
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyBase::HandleDeath()
{
	GameMode->EnemyDeath();
	GetWorld()->GetAuthGameMode<ARGGameMode>()->GainGold(GoldValue * GameMode->PlayerGoldMultiplier);
	Destroy();
}

void AEnemyBase::SetUpEnemy(USplineComponent *InSpline)
{
	Spline = InSpline;
	DistanceToReachBase = Spline->GetSplineLength();
	SetActorLocation(Spline->GetComponentLocation());
	ActorSpeed = ActorBaseSpeed;
}

void AEnemyBase::HandleBleed()
{
	UClass		*DamageTypeClass = UBleedDamageType::StaticClass();
	PreDamage += BleedDamage;
	UGameplayStatics::ApplyDamage
		(
			this,
			BleedDamage,
			nullptr,
			nullptr,
			DamageTypeClass
		);
}

void AEnemyBase::ApplySlow(float Damage)
{
	if (ActorSpeed >= (1.f - Damage) * ActorBaseSpeed)
	{
		if (GetWorldTimerManager().IsTimerActive(SlowTimerHandle))
		{
			GetWorldTimerManager().ClearTimer(SlowTimerHandle);
		}
		else
		{
			ActorSpeed = (1.f - Damage) * ActorSpeed;
			bIsSlowed = true;
		}
		OnSlowDelegate.Broadcast(1);
		GetWorldTimerManager().SetTimer(SlowTimerHandle, [this]()
		{
			bIsSlowed = false;
			ActorSpeed = ActorBaseSpeed;
			OnSlowDelegate.Broadcast(0);
		}, .5f, false);
	}
}

void AEnemyBase::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instgator,
                             AActor* DamageCauser)
{
	if (Cast<USlowDamageType>(DamageType))
	{
		ApplySlow(Damage);
	}
	else if (Cast<UBleedDamageType>(DamageType))
	{
		Health = MaxHealth - PreDamage;
		OnHealthChangeDelegate.Broadcast(Health);
		if (Health <= 0)
		{
			HandleDeath();
		}
	}
	else
	{
		if (AProjectile *Projectile = Cast<AProjectile>(DamageCauser))
		{
			//BLEEDING ATTACK
			if (Projectile->Bleed)
			{
				BleedDamage += Projectile->Bleed;
			}
			//STUN ATTACK
			// FMath::FRand() Generates a random value between 0 and 1
			if (Projectile->Stun >= 1.f || FMath::FRand() <= Projectile->Stun)
			{
				bIsStunned = true;
				GetWorldTimerManager().SetTimer(StunTimerHandle, [this]()
				{
					bIsStunned = false;
					OnUnstunDelegate.Broadcast(0);
				}, .5f, false);
			}
			//SLOW ATTACK
			if (Projectile->Slow)
			{
				ApplySlow(Projectile->Slow);
			}
		}
		Health = MaxHealth - PreDamage;
		OnHealthChangeDelegate.Broadcast(Health);
		if (Health <= 0)
		{
			HandleDeath();
		}
	}
}

