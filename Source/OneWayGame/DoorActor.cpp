#include "DoorActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "OneWayGameCharacter.h"
#include "Net/UnrealNetwork.h"

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
		}
	}
}

// En la función InteractWithDoor, modificar esta parte:

void ADoorActor::InteractWithDoor(AOneWayGameCharacter* InteractingPlayer)
{
	if (HasAuthority() && !bIsOpen && InteractingPlayer)
	{
		if (InteractingPlayer->GetHasKey())
		{
			bIsOpen = true;
			OnRep_IsOpen();
		}
		else
		{
			// Mostrar mensaje de que necesita la llave
			InteractingPlayer->ShowNeedKeyWidget();
		}
	}
}

void ADoorActor::OnRep_IsOpen()
{
	// Aquí implementas la animación/efecto de apertura de puerta
	if (DoorMesh)
	{
		// Por ejemplo, rotar la puerta o hacerla invisible
		if (bIsOpen)
		{
			DoorMesh->SetVisibility(false);
			DoorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

void ADoorActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ADoorActor, bIsOpen);
	DOREPLIFETIME(ADoorActor, DoorMesh);
	DOREPLIFETIME(ADoorActor, InteractionComponent);
}

void ADoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

