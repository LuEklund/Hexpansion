// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Projectile.h"

#include "NiagaraComponent.h"
#include "Enemy/EnemyBase.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	SetRootComponent(ProjectileMesh);


	TrailParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Smoke Trail"));
	TrailParticles->SetupAttachment(ProjectileMesh);

}


// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!IsValid(Target))
	{
		Destroy();
		return;
	}
	// Get the current and target actor locations
	FVector CurrentLocation = GetActorLocation();
	FVector TargetLocation = Target->GetActorLocation();
	
	// Calculate the direction to the target
	FVector Direction = TargetLocation - GetActorLocation();
	bool bImpact = false;
	if (Direction.Length() < ProximityImpact)
	{
		UClass		*DamageTypeClass = UDamageType::StaticClass();
		UGameplayStatics::ApplyDamage
		(
			Target,
			Damage,
			nullptr,
			this,
			DamageTypeClass
		);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation());
		bImpact = true;
		Destroy();
	}
	
	Direction.Normalize(); // Normalize the direction vector
	// Move the projectile towards the target
	float BonusSpeed = Target->ActorSpeed;
	if (BonusSpeed < 1.f)
	{
		BonusSpeed = 1.f;
	}
	FVector NewLocation = GetActorLocation() + (Direction * Speed * DeltaTime + Direction * BonusSpeed * DeltaTime);

	// Calculate the vectors to the target from the current and new locations
	FVector ToTargetFromCurrent = TargetLocation - CurrentLocation;
	FVector ToTargetFromNew = TargetLocation - NewLocation;


	// Check if the projectile has passed the target
	if (ToTargetFromCurrent.Length() < ToTargetFromNew.Length())
	{
		SetActorLocation(TargetLocation);
	}
	else
	{
		SetActorLocation(NewLocation);
	}
}

