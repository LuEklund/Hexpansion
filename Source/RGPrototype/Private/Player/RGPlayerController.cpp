// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/RGPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Abilities/AbilityBase.h"
#include "Blueprint/UserWidget.h"
#include "Buildings/BuildActor.h"
#include "Components/WidgetComponent.h"
#include "Enemy/EnemyBase.h"
#include "Game/RGGameMode.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Map/BuildableMapTile.h"
#include "Map/RGExpandButton.h"
#include "Player/PlayerPawn.h"
#include "UI/RGHUD.h"
#include "Waves/EnemySpawnSystem.h"


ARGPlayerController::ARGPlayerController()
{
}



void ARGPlayerController::BeginPlay()
{
	Super::BeginPlay();


	//Input subsystem setup
	UEnhancedInputLocalPlayerSubsystem *Subsystem =ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(MappingContext, 0);
	}
	
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);

	//Get Gamemode
	RGGameMode = GetWorld()->GetAuthGameMode<ARGGameMode>();
	
	//Initialize HUD
	if (ARGHUD *RPGHUD = Cast<ARGHUD>(GetHUD()))
	{
		RPGHUD->InitOverlay(this, RGGameMode);
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this; // No specific owner
	SpawnParams.Instigator = GetInstigator(); // No specific instigator
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	BuildingHighlight = GetWorld()->SpawnActor<ABuildActor>(BuildingClass, FVector(0.f,0.f, -1000.f), FRotator::ZeroRotator, SpawnParams);

}

void ARGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent *EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(ActionMove, ETriggerEvent::Triggered, this, &ARGPlayerController::Move);
	EnhancedInputComponent->BindAction(ActionEscape, ETriggerEvent::Triggered, this, &ARGPlayerController::Escape);
	EnhancedInputComponent->BindAction(ActionZoom, ETriggerEvent::Triggered, this, &ARGPlayerController::Zoom);
	EnhancedInputComponent->BindAction(ActionTestKeyP, ETriggerEvent::Triggered, this, &ARGPlayerController::InGameUpgarde);
	EnhancedInputComponent->BindAction(ActionTestKeyK, ETriggerEvent::Triggered, this, &ARGPlayerController::KillAll);
	EnhancedInputComponent->BindAction(ActionLeftClick, ETriggerEvent::Started, this, &ARGPlayerController::LeftClick);
	EnhancedInputComponent->BindAction(ActionRightClick, ETriggerEvent::Started, this, &ARGPlayerController::RightClick);
	EnhancedInputComponent->BindAction(ActionSellHotkey, ETriggerEvent::Started, this, &ARGPlayerController::SellHotkey);
	EnhancedInputComponent->BindAction(ActionUpgradeHotkey, ETriggerEvent::Started, this, &ARGPlayerController::UpgradeHotkey);
	EnhancedInputComponent->BindAction(ActionBackToOriginkey, ETriggerEvent::Started, this, &ARGPlayerController::BackToOriginkey);
	
	// InputComponent->BindAction("RightClick", IE_Pressed, this, &ARGPlayerController::HandleRightClick);
	
}

void ARGPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}

void ARGPlayerController::SetBuildType(FGameplayTag Type)
{
	BuildType = Type;
	// BuildingHighlight->SetDisplayAttackRange(RGGameMode->GetBuildingInfo(Type).AttackRange);
}



void ARGPlayerController::Move(const FInputActionValue& InputActionValue)
{
	FVector2d InputAxisVector = InputActionValue.Get<FVector2D>();

	const FRotator Rotator = GetControlRotation();
	const FRotator YawRotation(0.f, Rotator.Yaw, 0.f);

	const FVector	ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector	RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APlayerPawn *ControlledPawn = GetPawn<APlayerPawn>())
	{
		float CameraDistance = ControlledPawn->SpringArmComponent->TargetArmLength;
		// UE_LOG(LogTemp, Error, TEXT("1-length: %f"), CameraDistance);
		CameraDistance = CameraDistance / 100.f;
		// UE_LOG(LogTemp, Error, TEXT("2-length: %f"), CameraDistance);
		if (CameraDistance < 20.f) CameraDistance = 20.f;
		ControlledPawn->AddActorLocalOffset(ForwardDirection * InputAxisVector.Y * CameraDistance * GetWorld()->DeltaTimeSeconds * 100);
		ControlledPawn->AddActorLocalOffset(RightDirection * InputAxisVector.X * CameraDistance * GetWorld()->DeltaTimeSeconds * 100);

	}
}

void ARGPlayerController::Escape(const FInputActionValue& InputActionValue)
{
	if (!EscapeMenuWidget)
	{
		EscapeMenuWidget = NewObject<UUserWidget>(this, EscapeMenuWidgetClass);
	}
	if(!EscapeMenuWidget->IsInViewport())
	{
		EscapeMenuWidget->AddToViewport();
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
	// if(EscapeMenuWidget->IsInViewport())
	// {
	// 	EscapeMenuWidget->RemoveFromParent();
	// 	UGameplayStatics::SetGamePaused(GetWorld(), false);
	// }
	// else
	// {
	// 	EscapeMenuWidget->AddToViewport();
	// 	UGameplayStatics::SetGamePaused(GetWorld(), true);
	// }
}

void ARGPlayerController::Zoom(const FInputActionValue& InputActionValue)
{
	float InputAxisFloat = InputActionValue.Get<float>();
	// UE_LOG(LogTemp, Error, TEXT("InputAxisFloat: %f"), InputAxisFloat);
	if (APlayerPawn *ControlledPawn = GetPawn<APlayerPawn>())
	{
		float CameraDistance = ControlledPawn->SpringArmComponent->TargetArmLength;
		// UE_LOG(LogTemp, Error, TEXT("1-length: %f"), CameraDistance);
		CameraDistance = CameraDistance / 100.f;
		// UE_LOG(LogTemp, Error, TEXT("2-length: %f"), CameraDistance);
		if (CameraDistance < 20.f) CameraDistance = 20.f;
		
		ControlledPawn->SpringArmComponent->TargetArmLength = ControlledPawn->SpringArmComponent->TargetArmLength + ZoomSpeed * InputAxisFloat * CameraDistance; 
		if (ControlledPawn->SpringArmComponent->TargetArmLength < CameraMinZoom)
		{
			ControlledPawn->SpringArmComponent->TargetArmLength = CameraMinZoom;
		}
		else if (ControlledPawn->SpringArmComponent->TargetArmLength > CameraMaxZoom)
		{
			ControlledPawn->SpringArmComponent->TargetArmLength = CameraMaxZoom;
		}


		//adjust the plane opacity
		float PlaneOpacity = 1.5f - (ControlledPawn->SpringArmComponent->TargetArmLength / 10000.f);
		if (PlaneOpacity < 0.f) PlaneOpacity = 0.f;
		ControlledPawn->BackgroundPlaneOpacity(PlaneOpacity);

	}
}

void ARGPlayerController::Spawn(const FInputActionValue& InputActionValue)
{
	// UE_LOG(LogTemp, Error, TEXT("ERROR: Disabled for now. Enable in ARGPlayerController::Spawn line 90 ish"));
	RGGameMode->EnemySpawnSystem->SpawnWave();
}

void ARGPlayerController::KillAll(const FInputActionValue& InputActionValue)
{
	TArray<AActor*>	Enemies;
	UGameplayStatics::GetAllActorsOfClass(this, AEnemyBase::StaticClass(), Enemies);

	if (!Enemies.Num()) return;
	for (AActor *Enemy : Enemies)
	{
		Cast<AEnemyBase>(Enemy)->HandleDeath();
	}
	GetWorld()->GetTimerManager().ClearTimer(RGGameMode->EnemySpawnSystem->SpawnTimerHandle);
	RGGameMode->EnemySpawnSystem->EnemyGroups.Empty();
	RGGameMode->EnemySpawnSystem->SpawnNextEnemy();
	RGGameMode->EnemySpawnSystem->EnemiesAlive = 0;
	RGGameMode->EnemyDeath();

	//RGGameMode->EnemySpawnSystem->SpawnWave();
}

void ARGPlayerController::RightClick(const FInputActionValue& InputActionValue)
{
	// UE_LOG(LogTemp, Error, TEXT("Right: %d"), BuildType);
	BuildType = FGameplayTag();
	RGGameMode->BuildingCardDeselectAllDelegate.Broadcast(BuildType);
	RGGameMode->BuildingCardStatsChangeDelegate.Broadcast(BuildType);
	RGGameMode->TowerClickedDelegate.Broadcast(true);
	HideBuildActor();
}

void ARGPlayerController::LeftClick(const FInputActionValue& InputActionValue)
{
	if (!CursorHit.GetActor())
	{
		BuildType = FGameplayTag();
		HideBuildActor();
		RGGameMode->BuildingCardStatsChangeDelegate.Broadcast(BuildType);
		RGGameMode->TowerClickedDelegate.Broadcast(true);
		RGGameMode->CurrentSelectedTower  = nullptr;
		return ;
	}
	if (BuildType.IsValid())
	{
		ABuildableMapTile* Tile = Cast<ABuildableMapTile>(CursorHit.GetActor());
		if (Tile)
		{
			// UE_LOG(LogTemp, Error, TEXT("BuildType: %d"), BuildType);
			FBuilding &Type = RGGameMode->GetBuildingInfo(BuildType);
			if (RGGameMode->RequestGold(Type.Cost) == false)
				return ;
			if (ATower *NewTower = Tile->BuildTower(Type.TowerClass))
			{
				NewTower->Tags.AddUnique(Type.BuildingTag.GetTagName());
				NewTower->BuildingInfo = Type;
				
				FVector Min, Max;
				Tile->MeshComponent->GetLocalBounds(Min, Max);
				float Height = Max.Z - Min.Z;
				NewTower->BuildingInfo.AttackRange += Tile->Height * GetWorld()->GetAuthGameMode<ARGGameMode>()->HexWidth; 
				
				NewTower->SetTowerAttackSpeed(Type.AttackSpeed);
				if (NewTower->BuildingInfo.AttackAbilityClass)
				{
					NewTower->AttackAbility = NewObject<UAbilityBase>(this, NewTower->BuildingInfo.AttackAbilityClass);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("NO AbilityCLass in Tower"))
				}
				Type.Cost += Type.CostIncrease;
			}
			RGGameMode->UpdateBuildingMenu(BuildType);
			// Tile->BuildNotifyActorOnClicked(CursorHit.GetActor(), EKeys::LeftMouseButton);
		}
	}
	else if (ATower* Tower = Cast<ATower>(CursorHit.GetActor()))
	{
		RGGameMode->CurrentSelectedTower = Tower;
		
		BuildingHighlight->MeshComponent->SetMaterial(0, BuildingHighlight->Available);
		BuildingHighlight->SetDisplayAttackRange(Tower->BuildingInfo.AttackRange);
		BuildingHighlight->SetActorLocation(Tower->GetActorLocation());
		RGGameMode->TowerClickedDelegate.Broadcast(false);
		// UE_LOG(LogTemp, Error, TEXT("BuildType: %s"), *Tower->GetName());
	}
	else
	{
		HideBuildActor();
		RGGameMode->TowerClickedDelegate.Broadcast(true);
		RGGameMode->CurrentSelectedTower = nullptr;
	}
}

void ARGPlayerController::SellHotkey(const FInputActionValue& InputActionValue)
{
	if (IsValid(RGGameMode->CurrentSelectedTower))
	{
		RGGameMode->CurrentSelectedTower->Sell();
		HideBuildActor();
		RGGameMode->TowerClickedDelegate.Broadcast(true);
	}
	UE_LOG(LogTemp, Error, TEXT("TOWER IS NULL CANT SELL"));
}

void ARGPlayerController::UpgradeHotkey(const FInputActionValue& InputActionValue)
{
	if (IsValid(RGGameMode->CurrentSelectedTower))
	{
		RGGameMode->CurrentSelectedTower->UpgradeSelf();
	}
}

void ARGPlayerController::BackToOriginkey(const FInputActionValue& InputActionValue)
{
	if (APlayerPawn *ControlledPawn = GetPawn<APlayerPawn>())
	{
		FVector NewLocation = ControlledPawn->GetActorLocation();
		NewLocation.Y = 0.f;
		NewLocation.X = 0.f;
		ControlledPawn->SetActorLocation(FVector(NewLocation));

	}
}

void ARGPlayerController::InGameUpgarde(const FInputActionValue& InputActionValue)
{
	RGGameMode->OnInGameUpgrade.Broadcast();

}

void ARGPlayerController::HideBuildActor()
{
	BuildingHighlight->TextWidget->SetVisibility(false);
	BuildingHighlight->SetActorLocation(FVector(0.f,0.f,-1000.f));
}

void ARGPlayerController::CursorTrace()
{
	// UE_LOG(LogTemp, Error, TEXT("Building: %d"), bBuild);

	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (IsValid(CursorHit.GetActor()))
	{
		if (Cast<ABuildableMapTile>(CursorHit.GetActor()) && BuildType.IsValid())
		{
			BuildingHighlight->Update(CursorHit, BuildType);
		}
		// else if (Cast<ARGExpandButton>(CursorHit.GetActor()))
		// {
		// 	
		// }
		// UE_LOG(LogTemp, Error, TEXT("Hit Actor: %s"), *CursorHit.GetActor()->GetName());
	}
}
