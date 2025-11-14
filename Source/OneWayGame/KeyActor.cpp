// Fill out your copyright notice in the Description page of Project Settings.

#include "KeyActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "OneWayGameCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

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
	
	// SOLO replicar bIsCollected - los componentes ya se replican automáticamente
	DOREPLIFETIME(AKeyActor, bIsCollected);
}

// Called when the game starts or when spawned
void AKeyActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AKeyActor::OnOverlapBegin);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("KeyActor creado - bIsCollected: %s"), bIsCollected ? TEXT("True") : TEXT("False"));
}

void AKeyActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("KeyActor overlap con: %s"), *OtherActor->GetName());
	
	if (HasAuthority() && !bIsCollected)
	{
		AOneWayGameCharacter* PlayerCharacter = Cast<AOneWayGameCharacter>(OtherActor);
		if (PlayerCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("Jugador detectado, llamando CollectKey"));
			CollectKey(PlayerCharacter);
		}
	}
}

void AKeyActor::CollectKey(AOneWayGameCharacter* CollectingPlayer)
{
	if (HasAuthority() && !bIsCollected && CollectingPlayer)
	{
		bIsCollected = true;
		
		UE_LOG(LogTemp, Warning, TEXT("Llave recolectada por: %s"), *CollectingPlayer->GetName());
		
		// Notificar al jugador que recogió la llave
		CollectingPlayer->SetHasKey(true);
		
		// Ocultar la llave en todos los clientes
		OnRep_IsCollected();
		
		// Mensaje en pantalla para debug
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, 
				FString::Printf(TEXT(" %s recogió la llave!"), *CollectingPlayer->GetName()));
		}
	}
}

void AKeyActor::OnRep_IsCollected()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_IsCollected - bIsCollected: %s"), bIsCollected ? TEXT("True") : TEXT("False"));
	
	// Ocultar la malla y deshabilitar colisión cuando se recolecta
	if (KeyMesh)
	{
		KeyMesh->SetVisibility(!bIsCollected);
		UE_LOG(LogTemp, Warning, TEXT("KeyMesh visibility: %s"), (!bIsCollected) ? TEXT("Visible") : TEXT("Hidden"));
	}
	
	if (CollisionComponent)
	{
		CollisionComponent->SetCollisionEnabled(bIsCollected ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryOnly);
		UE_LOG(LogTemp, Warning, TEXT("CollisionComponent enabled: %s"), 
			(bIsCollected ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryOnly) == ECollisionEnabled::NoCollision ? 
			TEXT("NoCollision") : TEXT("QueryOnly"));
	}
}

// Called every frame
void AKeyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Rotación opcional para hacer la llave más visible
	if (!bIsCollected && KeyMesh)
	{
		KeyMesh->AddRelativeRotation(FRotator(0, 90.0f * DeltaTime, 0));
	}
}