// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/RGGameMode.h"

#include "Buildings/Tower.h"
#include "Components/WidgetComponent.h"
#include "Game/RG_GameInstance.h"
#include "Game/RG_GameState.h"
#include "Kismet/GameplayStatics.h"
#include "Map/RGExpandButton.h"
#include "Game/UpgradeManager.h"
#include "Map/RG_MapBase.h"
#include "UI/Widget/ButtonUserWidget.h"
#include "Waves/EnemySpawnSystem.h"
#include "Waves/WaveEndHandler.h"

ARGGameMode::ARGGameMode()
{
	// UpgradeManager = CreateDefaultSubobject<UUpgradeManager>("Upgrade Manager");
}

void ARGGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//GameState
	RGGameState = GetWorld()->GetGameState<ARG_GameState>();

	//In Game Upgrade Data
	UpgradeManager = NewObject<UUpgradeManager>(this, UUpgradeManager::StaticClass());
	UpgradeManager->RGGamemode = this;
	UpgradeManager->UnlockedInGameUpgrades = GetGameInstance<URG_GameInstance>()->InGameUpgradesData->GetUpgrades();

	// PlayerGold += GetGameInstance<URG_GameInstance>()->StartingBonusGoldMultiplier * 100;
	
	//Initialize spawner
	EnemySpawnSystem = GetWorld()->SpawnActor<AEnemySpawnSystem>(EnemySpawnSystemClass, FVector::Zero(), FRotator::ZeroRotator, SpawnParams);
	
	//Initialize Map
	MapSpawner = GetWorld()->SpawnActor<ARG_MapBase>(MapSpawnerClass, FVector::Zero(), FRotator::ZeroRotator, SpawnParams);
	AddExpandButtons();


	// //Initialize Button
	// ExpandButton = GetWorld()->SpawnActor<ARGExpandButton>(ExpandButtonClass, FVector::Zero(), FRotator::ZeroRotator, SpawnParams);
	// ExpandButton->ButtonWidgetComponent->OnClicked.AddDynamic(this, &ARGGameMode::Expand);

	//Initialize WaveHandler
	WaveEndHandler = GetWorld()->SpawnActor<AWaveEndHandler>(WaveEndHandlerClass, FVector::Zero(), FRotator::ZeroRotator, SpawnParams);

	ShowExpandButton();
	PlayerGoldChangeDelegate.Broadcast(PlayerGold);

	//Play music
	GetGameInstance<URG_GameInstance>()->PlayMusic(Music);

	//Building Data
	UBuildingData* NewBuildingDataInstance = DuplicateObject<UBuildingData>(BuildingData.Get(), GetTransientPackage());
	BuildingData = NewBuildingDataInstance;

	UGameplayStatics::PlaySound2D(this, StartSound);
	
	//In Game Upgrade Data
	// GetGameInstance<URG_GameInstance>()->InGameUpgradesData->InitUnlockedUpgrades();
	// UInGameUpgradesData* NewInGameUpgradesData = DuplicateObject<UInGameUpgradesData>(GetGameInstance<URG_GameInstance>()->InGameUpgradesData.Get(), GetTransientPackage());
	// InGameUpgradesData = NewInGameUpgradesData;
	// InGameUpgradesData = NewObject<UInGameUpgradesData>();
	// InGameUpgradesData->InitUnlockedUpgrades();
	// if (InGameUpgradesData)
	// {
	// 	InGameUpgradesData->InGameUpgradeCategories = GetGameInstance<URG_GameInstance>()->InGameUpgradesData->InGameUpgradeCategories();
	// }


}


void ARGGameMode::EnemyWaveSpawnAmount(int32 InAmount)
{
	WaveStartDelegate.Broadcast(InAmount);
}

void ARGGameMode::PlayerTakeDamage(int32 InDamage)
{
	PlayerHealth -= InDamage;
	PlayerHealthChangeDelegate.Broadcast(PlayerHealth);
	
	
	if (PlayerHealth <= 0)
	{
		// FString LevelName = LevelToLoad.GetAssetName();
		// UGameplayStatics::OpenLevel(this, FName(*LevelName));
		GameOverDelegate.Broadcast(false);

		// UGameplayStatics::SetGamePaused(GetWorld(), true);
		
		// Get the current world
		// UWorld* World = GetWorld();
		// if (World)
		// {
		// 	// Get the name of the current level
		// 	FString CurrentLevelName = World->GetMapName();
		// 	CurrentLevelName.RemoveFromStart(World->StreamingLevelsPrefix);
		//
		// 	// Reload the current level
		// 	UGameplayStatics::OpenLevel(World, FName(*CurrentLevelName));
		// }
	}
}


void ARGGameMode::UpdateBuildingMenu(FGameplayTag BuildType)
{
	BuildingCardStatsChangeDelegate.Broadcast(BuildType);
}



FBuilding& ARGGameMode::GetBuildingInfo(FGameplayTag InType)
{
	return BuildingData->GetBuildingByTag(InType);
}

void ARGGameMode::GainGold(int32 Gold)
{
	PlayerGold += Gold;
	PlayerGoldChangeDelegate.Broadcast(PlayerGold);
}

bool ARGGameMode::RequestGold(int32 Gold)
{
	if (PlayerGold < Gold)
		return false;
	PlayerGold -= Gold;
	PlayerGoldChangeDelegate.Broadcast(PlayerGold);
	return true;
}

bool ARGGameMode::CanAfford(FGameplayTag Build)
{
	return (GetBuildingInfo(Build).Cost <= PlayerGold);
}




void ARGGameMode::EnemyDeath()
{
	EnemySpawnSystem->EnemiesAlive--;
	EnemyDeathsDelegate.Broadcast(1);
	// UE_LOG(LogTemp, Error, TEXT("Enemies alive %d"),EnemySpawnSystem->EnemiesAlive)
	// if (EnemySpawnSystem->EnemiesAlive <= 0)
	// {
	// 	WaveEndHandler->GainXp();
	// 	if (++WaveEndHandler->CurrentWave == WaveEndHandler->MaxWaves)
	// 	{
	// 		// WaveEndHandler->HandleVictory( ); :TODO
	// 		
	// 	}
	// 	
	// 	if (WaveEndHandler->CurrentWave % WaveEndHandler->InGameUpgradeFrequency == 2)
	// 	{
	// 		UGameplayStatics::SetGamePaused(GetWorld(), true);
	// 		OnInGameUpgrade.Broadcast();
	// 	}
	// 	PlayerXpChangeDelegate.Broadcast(GetGameInstance<URG_GameInstance>()->PlayerXp);
	// }



	if (EnemySpawnSystem->EnemiesAlive <= 0)
	{
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EUpgradeType"), true);
		if (EnumPtr)
		{
			FString EnumDisplayName = EnumPtr->GetDisplayNameTextByValue(static_cast<int32>(PreviousUpgradeType)).ToString();
			UE_LOG(LogTemp, Error, TEXT("Type : %s"), *EnumDisplayName);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Type : Invalid Enum"));
		}
		
		WaveEndHandler->GainXp();
		if (++WaveEndHandler->CurrentWave == WaveEndHandler->MaxWaves)
		{
			GameOverDelegate.Broadcast(true);
		}
		else
		{
			UGameplayStatics::SetGamePaused(GetWorld(), true);
			OnInGameUpgrade.Broadcast();
			
			PlayerXpChangeDelegate.Broadcast(GetGameInstance<URG_GameInstance>()->PlayerXp);
			AddExpandButtons();
		}
		
	}
}



void ARGGameMode::ShowExpandButton()
{
	// // UE_LOG(LogTemp, Error, TEXT("Failed to spawn MapTile at location: %s"), *(MapSpawner->LastBase.Pos).ToString());
	// //TODO: move expand button!
	// ExpandButton->SetActorLocation(MapSpawner->GetEndNode());
	// ExpandButton->ButtonWidgetComponent->SetVisibility(true);
	// ExpandButton->ButtonWidgetComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}


int32 ARGGameMode::GetTowerCount()
{
	TArray<AActor *> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), Actors);
	return Actors.Num();
}

void ARGGameMode::Expand(UPrimitiveComponent* Component, FKey Key)
{
	if (!GetTowerCount())
	{
		ExpandPressedDelegate.Broadcast();
		return;
	}
	if (IsValid(CurrentSelectedTower))
	{
		return;
	}
	if (Component)
	{
		AActor* OwnerActor = Component->GetOwner();
		if (OwnerActor)
		{
			// Attempt to cast the owner actor to ARGExpandButton
			ARGExpandButton* ExpandButton = Cast<ARGExpandButton>(OwnerActor);
			if (ExpandButton)
			{
				PreviousUpgradeType = ExpandButton->MyType;
				MapSpawner->Expand(Component->GetOwner()->GetActorLocation());
				RemoveButtons();
				EnemySpawnSystem->SpawnWave();
				UGameplayStatics::PlaySound2D(GetWorld(), ExpandSound);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("BUTTON IS NOT ARGExpandButton"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Expand Component has no owner"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Expand Component is null"));
	}
}

void ARGGameMode::AddExpandButtons()
{
	std::vector<FVector> Positions;
	MapSpawner->GetPositions(Positions);
	if (Positions.empty())
		return ;
	for (FVector NewLocation : Positions)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		auto NewButton = GetWorld()->SpawnActor<ARGExpandButton>(ExpandButtonClass, NewLocation, FRotator::ZeroRotator, SpawnParams);
		int32 RandomNumber = FMath::RandRange(1, 100);
		EUpgradeType Type;
		if (RandomNumber > 80)
			Type = EUpgradeType::GOLD;
		else if (RandomNumber > 60)
			Type = EUpgradeType::SPECIAL_EFFECT;
		else if (RandomNumber >= 40)
			Type = EUpgradeType::DAMAGE;
		else if (RandomNumber >= 20)
			Type = EUpgradeType::UTILITY_ATTACK_RANGE;
		else
			Type = EUpgradeType::UTILITY_ATTACK_SPEED;
		if ((EnemySpawnSystem->WaveID + 1) % 5 == 0)
			Type = EUpgradeType::BOSS;
		// Type = EUpgradeType::SPECIAL_EFFECT;
		NewButton->SetMyType(Type);
		switch (Type)
		{
			case EUpgradeType::UTILITY_ATTACK_RANGE:
				Cast<UButtonUserWidget>(NewButton->ButtonWidgetComponent->GetWidget())->ButtonIcon->SetBrush(BrushAttackRange);
				break ;
			case EUpgradeType::UTILITY_ATTACK_SPEED:
				Cast<UButtonUserWidget>(NewButton->ButtonWidgetComponent->GetWidget())->ButtonIcon->SetBrush(BrushAttackSpeed);
				break ;
			case EUpgradeType::GOLD:
				Cast<UButtonUserWidget>(NewButton->ButtonWidgetComponent->GetWidget())->ButtonIcon->SetBrush(BrushGold);
				break ;
			case EUpgradeType::DAMAGE:
				Cast<UButtonUserWidget>(NewButton->ButtonWidgetComponent->GetWidget())->ButtonIcon->SetBrush(BrushDamage);
				break ;
			case EUpgradeType::SPECIAL_EFFECT:
				Cast<UButtonUserWidget>(NewButton->ButtonWidgetComponent->GetWidget())->ButtonIcon->SetBrush(SpecialEffect);
				break ;
			case EUpgradeType::BOSS:
				Cast<UButtonUserWidget>(NewButton->ButtonWidgetComponent->GetWidget())->ButtonIcon->SetBrush(BrushBoss);
				break ;
			default:
				Cast<UButtonUserWidget>(NewButton->ButtonWidgetComponent->GetWidget())->ButtonIcon->SetBrush(BrushAttackRange);
				break ;
		}
		NewButton->ButtonWidgetComponent->OnClicked.AddDynamic(this, &ARGGameMode::Expand);
		NewButton->ButtonWidgetComponent->SetVisibility(true);
		NewButton->ButtonWidgetComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		ExpandButtons.Add(NewButton);
	}
}

void ARGGameMode::RemoveButtons()
{
	for (auto e : ExpandButtons)
	{
		if (IsValid(e))
			e->Destroy();
	}
		
}

void ARGGameMode::Upgrade(FInGameUpgradeRow &UpgradeInfo)
{
	UpgradeManager->Upgrade(UpgradeInfo);
	if (CurrentSelectedTower)
	{
		TowerClickedDelegate.Broadcast(false);
	}

}
