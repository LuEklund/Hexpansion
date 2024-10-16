// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildings/Nexus.h"

#include "Enemy/EnemyBase.h"
#include "Game/RGGameMode.h"

// Sets default values
ANexus::ANexus()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NexusMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Nexus");
	SetRootComponent(NexusMeshComponent);

}

// Called when the game starts or when spawned
void ANexus::BeginPlay()
{
	Super::BeginPlay();

	NexusMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ANexus::BeginOverlap);
	GameMode = GetWorld()->GetAuthGameMode<ARGGameMode>();
	
}

// Called every frame
void ANexus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANexus::BeginOverlap(UPrimitiveComponent *OnComponentBeginOverlap,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		if (AEnemyBase *Enemy = Cast<AEnemyBase>(OtherActor))
		{
			if (Enemy->MaxHealth > Enemy->PreDamage)
			{
				// if (Enemy->DistanceTraveled > 10.f) return;
				GameMode->PlayerTakeDamage(1);
				Enemy->HandleDeath();
			}
		}
	}
}


// void ANexus::BeginOverlap(UPrimitiveComponent OnComponentBeginOverlap,
//                           AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
//                           const FHitResult& SweepResult)
// {
//
// }

// void ANexus::NotifyActorBeginOverlap(AActor* OtherActor)
// {
// 	Super::NotifyActorBeginOverlap(OtherActor);
// 	UE_LOG(LogTemp, Error, TEXT("Overlap WIth: %s"), *OtherActor->GetName())
// }

