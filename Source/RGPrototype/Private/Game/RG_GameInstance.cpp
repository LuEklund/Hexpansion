// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/RG_GameInstance.h"

#include "Components/AudioComponent.h"
#include "data/InGameUpgradesData.h"
#include "Game/RGSaveGame.h"
#include "Kismet/GameplayStatics.h"



void URG_GameInstance::Init()
{
	Super::Init();
	UInGameUpgradesData* NewInGameUpgradesData = DuplicateObject<UInGameUpgradesData>(InGameUpgradesData.Get(), GetTransientPackage());
	InGameUpgradesData = NewInGameUpgradesData;

	LoadGameData(false);
	AsyncLoadGameFromSlotDelegate.BindUObject(this, &URG_GameInstance::OnLoadGameFromSlotComplete);

}


void URG_GameInstance::StopMusic()
{
	if (MusicAudioComponent)
	{
		MusicAudioComponent->Stop();
		MusicAudioComponent->DestroyComponent();
	}
}

void URG_GameInstance::PlayMusic(USoundBase *Music)
{
	// Ensure Music is set
	if (Music)
	{
		if (MusicAudioComponent)
		{
			MusicAudioComponent->Stop();
			MusicAudioComponent->DestroyComponent();
		}
		MusicAudioComponent = UGameplayStatics::SpawnSound2D(this, Music, 1.f, 1.f,0.f, nullptr, true, false);
		MusicAudioComponent->Play();
		// else if (!MusicAudioComponent->IsPlaying())
		// {
		// 	// If it's not playing, play it
		// 	MusicAudioComponent->Play();
		// }
	}
}

FGameplayTag URG_GameInstance::UnlockInGameUpgrade(FName Name)
{
	FInGameUpgradeRow* Upgrade = InGameUpgradesData->GetUpgradeByName(Name);
	if (!Upgrade) return FGameplayTag();

	if (Upgrade->UnlockXPCost <= PlayerXp)
	{
		Upgrade->State = FRGGameplayTags::Get().State_Unlocked;
		PlayerXp -= Upgrade->UnlockXPCost;
	}
	return Upgrade->State;
}


void URG_GameInstance::OnLoadGameFromSlotComplete(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGame)
{
	SaveGame = Cast<URGSaveGame>(LoadedGame);
	
	PlayerXp = SaveGame->PlayerStats.PlayerXP;
}

void URG_GameInstance::LoadGameData(bool bAsync)
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		if (bAsync)
		{
			UGameplayStatics::AsyncLoadGameFromSlot(SaveSlotName, 0, AsyncLoadGameFromSlotDelegate);
		}
		else
		{
			SaveGame = Cast<URGSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
		}
		//After loading Save update stats
		PlayerXp = SaveGame->PlayerStats.PlayerXP;
		StartingBonusGoldMultiplier = SaveGame->PlayerStats.BonusGoldMultiplier;
	}
	else
	{
		SaveGame = Cast<URGSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
	}
}

void URG_GameInstance::SaveGameData(bool bAsync)
{
	if (bAsync)
	{
		UGameplayStatics::AsyncSaveGameToSlot(SaveGame, SaveSlotName, 0);
	}
	else
	{
		UGameplayStatics::SaveGameToSlot(SaveGame, SaveSlotName, 0);
	}
}

URGSaveGame* URG_GameInstance::GetGameData()
{
	return nullptr;
}

void URG_GameInstance::SavePlayerStats(FPlayerStats Stats, bool bAsync)
{
	SaveGame->PlayerStats = Stats;
	SaveGameData(bAsync);
}

void URG_GameInstance::SavePermaUpgrades(bool bAsync)
{
	SaveGame->UnlockedUpgrades.Empty();
	SaveGameData(bAsync);
}

void URG_GameInstance::Shutdown()
{
	SaveGame->PlayerStats.PlayerXP = PlayerXp;
	SaveGame->PlayerStats.BonusGoldMultiplier = StartingBonusGoldMultiplier;
	SaveGameData(false);
	SavePermaUpgrades(false);
	Super::Shutdown();
}

