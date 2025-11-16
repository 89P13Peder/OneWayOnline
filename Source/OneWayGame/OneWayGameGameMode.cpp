// Copyright Epic Games, Inc. All Rights Reserved.

#include "OneWayGameGameMode.h"

#include "OneWayGameCharacter.h"
#include "OneWayGameGameStateBase.h"
#include "AI/NavigationSystemBase.h"
#include "Kismet/GameplayStatics.h"

AOneWayGameGameMode::AOneWayGameGameMode()
{
	// Configura el GameState class
	GameStateClass = AOneWayGameGameStateBase::StaticClass();
    
	UE_LOG(LogTemp, Warning, TEXT("GameMode constructor - GameStateClass configured"));
}

void AOneWayGameGameMode::EvaluatePlayers()
{
	UWorld* World = GetWorld();
	if (!World) return;

	TArray<AActor*> FoundPlayers;
	UGameplayStatics::GetAllActorsOfClass(World, AOneWayGameCharacter::StaticClass(), FoundPlayers);
	UE_LOG(LogTemp, Warning, TEXT("EvaluatePlayers: Encontrados %d jugadores"), FoundPlayers.Num());


	for (AActor* Player : FoundPlayers)
	{
		AOneWayGameCharacter* OWPlayer  = Cast<AOneWayGameCharacter>(Player);
		if (!OWPlayer) continue;
		
		UE_LOG(LogTemp, Warning, TEXT("Evaluando jugador: %s"), *OWPlayer->GetName());
		UE_LOG(LogTemp, Warning, TEXT("Jugador tiene llave: %hhd"), OWPlayer->GetHasKey());
		if (OWPlayer->GetHasKey())
		{
			OWPlayer->ShowWinWidget();
		}
		else if (!OWPlayer->GetHasKey())
		{
			OWPlayer->ShowLoseWidget();
		}
	}
}
