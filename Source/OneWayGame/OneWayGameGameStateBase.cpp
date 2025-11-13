// Fill out your copyright notice in the Description page of Project Settings.


#include "OneWayGameGameStateBase.h"

#pragma once

UCLASS()
class ONEWAYGAME_API AMyGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(ReplicatedUsing = OnRep_Winner)
	APlayerState* Winner;

	UFUNCTION()
	void OnRep_Winner();

	void SetWinner(APlayerState* NewWinner);
};