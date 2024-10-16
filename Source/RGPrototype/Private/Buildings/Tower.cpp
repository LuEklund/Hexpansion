// Fill out your copyright notice in the Description page of Project Settings.

#include "Buildings/Tower.h"
#include "Abilities/AbilityBase.h"
#include "AI/RogueAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/EnemyBase.h"
#include "Game/RGGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Map/BuildableMapTile.h"

// Sets default values
ATower::ATower()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TowerBase = CreateDefaultSubobject<UStaticMeshComponent>("TowerBase");
	SetRootComponent(TowerBase);

	TowerTop = CreateDefaultSubobject<UStaticMeshComponent>("TowerTop");
	TowerTop->SetupAttachment(TowerBase);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>("ProjectileSpawnPoint");
	ProjectileSpawnPoint->SetupAttachment(TowerTop);

}

// Called when the game starts or when spawned
void ATower::BeginPlay()
{
	Super::BeginPlay();

}

void ATower::SetTowerAttackSpeed(float InAttackSpeed)
{
	BuildingInfo.AttackSpeed = InAttackSpeed;
	if (BuildingInfo.AttackSpeed  < 0.01f) BuildingInfo.AttackSpeed  = 0.01f;
	RogueAIController->GetBlackboardComponent()->SetValueAsFloat(TEXT("AttackSpeed"), 1.f/BuildingInfo.AttackSpeed );
}

float ATower::GetTowerAttackSpeed()
{
	return BuildingInfo.AttackSpeed ;
}

bool ATower::UpgradeSelf()
{
	ARGGameMode *GameMode = GetWorld()->GetAuthGameMode<ARGGameMode>();
	if (BuildingInfo.UpgradeTable)
	{
		FString PrevLevelString = FString::FromInt(BuildingInfo.Level);
		FString LevelString = FString::FromInt(BuildingInfo.Level + 1);
		FName RowName = FName(*LevelString); // Replace with the appropriate row name
		FName PrevRowName = FName(*PrevLevelString); // Replace with the appropriate row name

		const FString ContextString(TEXT("GENERAL"));
		FUpgradeTowerRow* UpgradeRow = BuildingInfo.UpgradeTable->FindRow<FUpgradeTowerRow>(RowName, ContextString, true);
		FUpgradeTowerRow* PrevUpgradeRow = BuildingInfo.UpgradeTable->FindRow<FUpgradeTowerRow>(PrevRowName, ContextString, true);

		if (UpgradeRow && PrevUpgradeRow && GameMode->RequestGold(UpgradeRow->Cost))
		{
			// UE_LOG(LogTemp, Warning, TEXT("Cost: %d, Damage: %f, AttackSpeed: %f, Range: %f"),
			// 	UpgradeRow->Cost, UpgradeRow->Damage, UpgradeRow->AttackSpeed, UpgradeRow->Range);
			// UE_LOG(LogTemp, Warning, TEXT("Cost: %d, Damage: %f, AttackSpeed: %f, Range: %f"),
   //          				PrevUpgradeRow->Cost, PrevUpgradeRow->Damage, PrevUpgradeRow->AttackSpeed, PrevUpgradeRow->Range);

			// Example of using the data to set building properties
			BuildingInfo.Cost += UpgradeRow->Cost;
			BuildingInfo.AttackDamage += UpgradeRow->Damage - PrevUpgradeRow->Damage;
			BuildingInfo.AttackRange += UpgradeRow->Range - PrevUpgradeRow->Range;
			BuildingInfo.Bleed += UpgradeRow->Bleed - PrevUpgradeRow->Bleed;
			BuildingInfo.Stun += UpgradeRow->Stun - PrevUpgradeRow->Stun;
			BuildingInfo.Slow += UpgradeRow->Slow - PrevUpgradeRow->Slow;
			SetTowerAttackSpeed(BuildingInfo.AttackSpeed + UpgradeRow->AttackSpeed - PrevUpgradeRow->AttackSpeed);
			BuildingInfo.Level++;

			GameMode->TowerClickedDelegate.Broadcast(false);
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Row not found!"));
			return false;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UpgradeTable is not assigned!"));
		return false;
	}
}

void ATower::Sell()
{
	//TODO: sell for 60-70% and have the value and percentage displayed in the info widget.
	if (IsValid(this))
	{
		ARGGameMode *RGGamemode = GetWorld()->GetAuthGameMode<ARGGameMode>();
		RGGamemode->GainGold(BuildingInfo.Cost);
		RGGamemode->GetBuildingInfo(BuildingInfo.BuildingTag).Cost -= RGGamemode->GetBuildingInfo(BuildingInfo.BuildingTag).CostIncrease;
		RGGamemode->UpdateBuildingMenu(BuildingInfo.BuildingTag);
		OwnerTile->bHasTower = false;
		Destroy();
	}
}

// Called every frame
void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// DrawDebugSphere(GetWorld(), GetActorLocation(), TowerRange, 16, FColor::Red, false, 0.1f, 0, 3.f);


}

// Called to bind functionality to input
void ATower::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATower::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	RogueAIController = Cast<ARogueAIController>(NewController);
	//Setup behaviour tree
	RogueAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);

	
	RogueAIController->RunBehaviorTree(BehaviorTree);
}

void ATower::Fire(AActor* Target)
{
	if (AttackAbility)
	{
		if (AttackSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), AttackSound, GetActorLocation());
		}
		AttackAbility->ExecuteAbility(Cast<AEnemyBase>(Target), this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Tower fire default overrite function Fire"));
	}
}

