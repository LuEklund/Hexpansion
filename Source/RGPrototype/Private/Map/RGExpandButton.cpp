// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/RGExpandButton.h"

#include "Components/WidgetComponent.h"

void ARGExpandButton::SetMyType(EUpgradeType Type)
{
	MyType = Type;
}

// Sets default values
ARGExpandButton::ARGExpandButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = RootSceneComponent;
	
	ButtonWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Button WIdget");
	ButtonWidgetComponent->SetupAttachment(RootSceneComponent);

	InfoWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Info Widget");
	InfoWidgetComponent->SetupAttachment(RootSceneComponent);

	// Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	// Mesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ARGExpandButton::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ARGExpandButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

