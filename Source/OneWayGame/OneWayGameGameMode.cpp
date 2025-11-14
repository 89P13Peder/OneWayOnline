// Copyright Epic Games, Inc. All Rights Reserved.

#include "OneWayGameGameMode.h"
#include "OneWayGameGameStateBase.h"

AOneWayGameGameMode::AOneWayGameGameMode()
{
	// Configura el GameState class
	GameStateClass = AOneWayGameGameStateBase::StaticClass();
    
	UE_LOG(LogTemp, Warning, TEXT("GameMode constructor - GameStateClass configured"));
}
