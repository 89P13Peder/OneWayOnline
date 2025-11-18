// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "OneWayGameGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class ONEWAYGAME_API AOneWayGameGameStateBase : public AGameStateBase
{
    GENERATED_BODY()

public:
    AOneWayGameGameStateBase();

    UPROPERTY(ReplicatedUsing = OnRep_Winner)
    APlayerState* Winner;
    
    UPROPERTY(ReplicatedUsing = OnRep_TimeLeft, BlueprintReadOnly, Category = "Game State")
    float TimeLeft;

    UFUNCTION(BlueprintCallable, Category = "Game State")
    void SetTimeLeft(float NewTimeLeft);

    // Función OnRep — se ejecuta cuando TimeLeft cambia
    UFUNCTION()
    void OnRep_TimeLeft();

    UFUNCTION()
    void OnRep_Winner();

    void SetWinner(APlayerState* NewWinner);

    // NUEVA FUNCIÓN - usa un nombre diferente para el parámetro
    UFUNCTION(BlueprintCallable, Category = "Game State")
    void NotifyPlayerWin(class AOneWayGameCharacter* WinningPlayer); // Cambiado de 'Winner' a 'WinningPlayer'

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};