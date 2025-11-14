#include "WinZoneActor.h"
#include "Components/BoxComponent.h"
#include "OneWayGameCharacter.h"
#include "OneWayGameGameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

AWinZoneActor::AWinZoneActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	WinZoneCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WinZoneCollision"));
	RootComponent = WinZoneCollision;
	WinZoneCollision->SetBoxExtent(FVector(200.0f, 200.0f, 200.0f));
	WinZoneCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void AWinZoneActor::BeginPlay()
{
    Super::BeginPlay();
    
    // Mensaje en pantalla (siempre visible)
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,                    // Key (-1 para no reemplazar)
            10.0f,                 // Tiempo en pantalla
            FColor::Green,         // Color
            TEXT("WIN ZONE: BeginPlay ejecutado") // Mensaje
        );
    }
    
    UE_LOG(LogTemp, Warning, TEXT("WinZone BeginPlay - Revisa Unit Output en Rider"));
    
    if (HasAuthority())
    {
        WinZoneCollision->OnComponentBeginOverlap.AddDynamic(this, &AWinZoneActor::OnOverlapBegin);
        
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, 
                TEXT("WIN ZONE: Overlap events configurados (AUTORIDAD)"));
        }
    }
}

void AWinZoneActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Mensaje en pantalla para ANY overlap
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange,
            FString::Printf(TEXT("OVERLAP DETECTADO con: %s"), *OtherActor->GetName()));
    }
    
    UE_LOG(LogTemp, Warning, TEXT("=== OVERLAP DETECTADO ==="));
    UE_LOG(LogTemp, Warning, TEXT("Actor: %s"), *OtherActor->GetName());
    UE_LOG(LogTemp, Warning, TEXT("Clase: %s"), *OtherActor->GetClass()->GetName());
    
    // Verifica si es el personaje del jugador
    AOneWayGameCharacter* PlayerCharacter = Cast<AOneWayGameCharacter>(OtherActor);
    if (PlayerCharacter)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green,
                FString::Printf(TEXT(" ES JUGADOR: %s"), *PlayerCharacter->GetName()));
        }
        
        UE_LOG(LogTemp, Warning, TEXT(" Es un jugador: %s"), *PlayerCharacter->GetName());
        
        if (!PlayersWhoWon.Contains(PlayerCharacter))
        {
            PlayersWhoWon.Add(PlayerCharacter);
            
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan,
                    TEXT(" JUGADOR GANADOR DETECTADO!"));
            }
            
            UE_LOG(LogTemp, Warning, TEXT(" Jugador agregado a lista de ganadores: %s"), *PlayerCharacter->GetName());
            
            // Usar GameState para manejar la lógica de ganar/perder
            UWorld* World = GetWorld();
            if (World)
            {
                AOneWayGameGameStateBase* GameState = World->GetGameState<AOneWayGameGameStateBase>();
                if (GameState)
                {
                    UE_LOG(LogTemp, Warning, TEXT(" GameState encontrado, llamando a NotifyPlayerWin"));
                    GameState->NotifyPlayerWin(PlayerCharacter);
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT(" GameState es null"));
                    
                    // Fallback: mostrar widgets directamente si GameState no funciona
                    if (GEngine)
                    {
                        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red,
                            TEXT(" Fallback: Mostrando widgets directamente"));
                    }
                    
                    // Mostrar WinWidget al ganador
                    PlayerCharacter->ShowWinWidget();
                    
                    // Mostrar LoseWidget a otros jugadores
                    for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
                    {
                        APlayerController* PC = It->Get();
                        if (PC && PC->GetPawn())
                        {
                            AOneWayGameCharacter* OtherPlayer = Cast<AOneWayGameCharacter>(PC->GetPawn());
                            if (OtherPlayer && OtherPlayer != PlayerCharacter)
                            {
                                UE_LOG(LogTemp, Warning, TEXT("Mostrando LoseWidget a: %s"), *OtherPlayer->GetName());
                                OtherPlayer->ShowLoseWidget();
                            }
                        }
                    }
                }
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT(" World es null"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT(" Jugador ya estaba en la lista de ganadores"));
            
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow,
                    TEXT("⚠ Este jugador ya ganó antes"));
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT(" No es un jugador, es: %s"), *OtherActor->GetClass()->GetName());
        
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red,
                FString::Printf(TEXT(" No es jugador: %s"), *OtherActor->GetClass()->GetName()));
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("=== FIN OVERLAP ==="));
}

// Agrega esta nueva función a WinZoneActor
void AWinZoneActor::ShowLoseWidgetToOtherPlayers(AOneWayGameCharacter* Winner)
{
    if (!HasAuthority()) return;
    
    UWorld* World = GetWorld();
    if (!World) return;
    
    UE_LOG(LogTemp, Warning, TEXT("Buscando otros jugadores para mostrar LoseWidget..."));
    
    // Buscar todos los personajes en el nivel
    for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (PC && PC->GetPawn())
        {
            AOneWayGameCharacter* OtherPlayer = Cast<AOneWayGameCharacter>(PC->GetPawn());
            if (OtherPlayer && OtherPlayer != Winner)
            {
                UE_LOG(LogTemp, Warning, TEXT("Mostrando LoseWidget a: %s"), *OtherPlayer->GetName());
                OtherPlayer->ShowLoseWidget();
            }
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