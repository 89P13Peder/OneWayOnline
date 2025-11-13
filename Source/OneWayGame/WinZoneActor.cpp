#include "WinZoneActor.h"
#include "Components/BoxComponent.h"
#include "OneWayGameCharacter.h"
#include "Net/UnrealNetwork.h"

AWinZoneActor::AWinZoneActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	// Crear componente de colisión
	WinZoneCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WinZoneCollision"));
	RootComponent = WinZoneCollision;
	WinZoneCollision->SetBoxExtent(FVector(200.0f, 200.0f, 200.0f));
	WinZoneCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void AWinZoneActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		WinZoneCollision->OnComponentBeginOverlap.AddDynamic(this, &AWinZoneActor::OnOverlapBegin);
	}
}

void AWinZoneActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		AOneWayGameCharacter* PlayerCharacter = Cast<AOneWayGameCharacter>(OtherActor);
		if (PlayerCharacter && !PlayersWhoWon.Contains(PlayerCharacter))
		{
			// Agregar jugador a la lista de ganadores
			PlayersWhoWon.Add(PlayerCharacter);
			
			// Mostrar widget de victoria solo a este jugador
			PlayerCharacter->ShowWinWidget();
			
			// Mostrar widget de derrota a todos los demás jugadores
			for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
			{
				APlayerController* PC = It->Get();
				if (PC && PC->GetPawn() != PlayerCharacter)
				{
					if (AOneWayGameCharacter* OtherPlayer = Cast<AOneWayGameCharacter>(PC->GetPawn()))
					{
						OtherPlayer->ShowLoseWidget();
					}
				}
			}
			
			UE_LOG(LogTemp, Log, TEXT("Player %s reached the win zone!"), *PlayerCharacter->GetName());
		}
	}
}

void AWinZoneActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AWinZoneActor, PlayersWhoWon);
}

void AWinZoneActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

