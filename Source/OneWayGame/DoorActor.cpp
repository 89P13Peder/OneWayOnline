#include "DoorActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "OneWayGameCharacter.h"
#include "OneWayGameGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

class AOneWayGameGameMode;

ADoorActor::ADoorActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	// Componente de malla
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	RootComponent = DoorMesh;

	// Componente de interacción
	InteractionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionComponent"));
	InteractionComponent->SetupAttachment(RootComponent);
	InteractionComponent->SetBoxExtent(FVector(100.0f, 50.0f, 200.0f));
	InteractionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	bIsOpen = false;
}

void ADoorActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		InteractionComponent->OnComponentBeginOverlap.AddDynamic(this, &ADoorActor::OnOverlapBegin);
	}
}

void ADoorActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		AOneWayGameCharacter* PlayerCharacter = Cast<AOneWayGameCharacter>(OtherActor);
		if (PlayerCharacter && !bIsOpen)
		{
			InteractWithDoor(PlayerCharacter);
			ServerWinCondition();
		}
	}
}

void ADoorActor::InteractWithDoor(AOneWayGameCharacter* InteractingPlayer)
{
	if (HasAuthority() && !bIsOpen && InteractingPlayer)
	{
		if (InteractingPlayer->GetHasKey())
		{
			bIsOpen = true;
			OnRep_IsOpen(); // Llamar manualmente en servidor
			
			UE_LOG(LogTemp, Warning, TEXT("Puerta abierta por jugador con llave"));
		}
		else
		{
			// Mostrar mensaje de que necesita la llave
			InteractingPlayer->ShowNeedKeyWidget();
			UE_LOG(LogTemp, Warning, TEXT("Jugador necesita llave para abrir puerta"));
		}
	}
}

void ADoorActor::OnRep_IsOpen()
{
	// Aquí implementas la animación/efecto de apertura de puerta
	if (DoorMesh)
	{
		if (bIsOpen)
		{
			DoorMesh->SetVisibility(false);
			DoorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			InteractionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		else
		{
			DoorMesh->SetVisibility(true);
			DoorMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			InteractionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
	}
}

void ADoorActor::ServerWinCondition_Implementation()
{
	if (!HasAuthority()) return; // Seguridad

	// Obtener GameMode
	AOneWayGameGameMode* GM = Cast<AOneWayGameGameMode>(UGameplayStatics::GetGameMode(this));
	if (GM)
	{
		GM->EvaluatePlayers();	
	}
}

void ADoorActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ADoorActor, bIsOpen);
	// NO replicar DoorMesh e InteractionComponent - ya se replican automáticamente
}

void ADoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}