// Fill out your copyright notice in the Description page of Project Settings.

#include "KeyActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "OneWayGameCharacter.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AKeyActor::AKeyActor()
{
 	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Hacer que este actor se replique
	bReplicates = true;
	
	// Crear componente de colisión
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->SetSphereRadius(50.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	
	// Crear componente de malla
	KeyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMesh"));
	KeyMesh->SetupAttachment(RootComponent);
	KeyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	bIsCollected = false;
}

void AKeyActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AKeyActor, bIsCollected);
	DOREPLIFETIME(AKeyActor, KeyMesh);
	DOREPLIFETIME(AKeyActor, CollisionComponent);
}

// Called when the game starts or when spawned
void AKeyActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AKeyActor::OnOverlapBegin);
	}
}

void AKeyActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority() && !bIsCollected)
	{
		AOneWayGameCharacter* PlayerCharacter = Cast<AOneWayGameCharacter>(OtherActor);
		if (PlayerCharacter)
		{
			CollectKey(PlayerCharacter);
		}
	}
}

void AKeyActor::CollectKey(AOneWayGameCharacter* CollectingPlayer)
{
	if (HasAuthority() && !bIsCollected)
	{
		bIsCollected = true;
		
		// Notificar al jugador que recogió la llave
		if (CollectingPlayer)
		{
			CollectingPlayer->SetHasKey(true);
		}
		
		// Ocultar la llave en todos los clientes
		OnRep_IsCollected();
	}
}

void AKeyActor::OnRep_IsCollected()
{
	// Ocultar la malla y deshabilitar colisión cuando se recolecta
	if (KeyMesh)
	{
		KeyMesh->SetVisibility(!bIsCollected);
	}
	if (CollisionComponent)
	{
		CollisionComponent->SetCollisionEnabled(bIsCollected ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryOnly);
	}
}

// Called every frame
void AKeyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}