#include "OneWayGameGameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerState.h"
#include "OneWayGameCharacter.h" // Necesitas incluir esto

AOneWayGameGameStateBase::AOneWayGameGameStateBase()
{
    Winner = nullptr;
}

void AOneWayGameGameStateBase::OnRep_TimeLeft()
{
    UE_LOG(LogTemp, Warning, TEXT("TimeLeft ha cambiado: %f"), TimeLeft);
}

void AOneWayGameGameStateBase::OnRep_Winner()
{
    // Lógica cuando el ganador cambia
    UE_LOG(LogTemp, Warning, TEXT("Ganador actualizado: %s"), 
        Winner ? *Winner->GetPlayerName() : TEXT("Ninguno"));
}

void AOneWayGameGameStateBase::SetWinner(APlayerState* NewWinner)
{
    if (HasAuthority())
    {
        Winner = NewWinner;
        OnRep_Winner();
    }
}

// IMPLEMENTACIÓN DE LA NUEVA FUNCIÓN
void AOneWayGameGameStateBase::NotifyPlayerWin(AOneWayGameCharacter* WinningPlayer)
{
    if (!HasAuthority()) 
    {
        UE_LOG(LogTemp, Warning, TEXT("NotifyPlayerWin llamado sin autoridad"));
        return;
    }
    
    if (!WinningPlayer)
    {
        UE_LOG(LogTemp, Error, TEXT("NotifyPlayerWin: WinningPlayer es null"));
        return;
    }
    
    UE_LOG(LogTemp, Warning, TEXT("NotifyPlayerWin: %s ganó el juego"), 
        *WinningPlayer->GetName());
    
    // Establecer el ganador en el GameState
    SetWinner(WinningPlayer->GetPlayerState());
    
    UWorld* World = GetWorld();
    if (!World) 
    {
        UE_LOG(LogTemp, Error, TEXT("NotifyPlayerWin: World es null"));
        return;
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Buscando jugadores para notificar..."));
    
    // Notificar a todos los jugadores
    for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (PC && PC->GetPawn())
        {
            AOneWayGameCharacter* Player = Cast<AOneWayGameCharacter>(PC->GetPawn());
            if (Player)
            {
                if (Player == WinningPlayer)
                {
                    UE_LOG(LogTemp, Warning, TEXT("Mostrando WinWidget a: %s"), *Player->GetName());
                    Player->ShowWinWidget();
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Mostrando LoseWidget a: %s"), *Player->GetName());
                    Player->ShowLoseWidget();
                }
            }
        }
    }
}

void AOneWayGameGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(AOneWayGameGameStateBase, Winner);
}