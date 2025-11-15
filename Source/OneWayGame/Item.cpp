// Fill out your copyright notice in the Description page of Project Settings.
#include "Item.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	// Crear un Root (opcional, si no quieres usar el Trigger como root)
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// Crear Box Trigger
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(RootComponent);
	Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Trigger->SetCollisionResponseToAllChannels(ECR_Overlap); // equivale a OverlapAllDynamic
	Trigger->SetGenerateOverlapEvents(true);

	// Crear Static Mesh (NO hijo del Trigger)
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(RootComponent); // ⬅ Aquí el cambio importante
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // para que no interfiera
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

