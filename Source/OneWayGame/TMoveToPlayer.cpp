// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "TMoveToPlayer.h"

#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

UTMoveToPlayer::UTMoveToPlayer()
{
	NodeName = TEXT("Moverse hacia el jugador");
}

EBTNodeResult::Type UTMoveToPlayer::ExecuteTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner());
	if (!AIController) return EBTNodeResult::Failed;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(AIController, 0); // ESTO SOLO FUNCIONA PARA EL SERVIDOR.
	if (!PlayerPawn) return EBTNodeResult::Failed;

	AIController->MoveToActor(PlayerPawn, 5.0f);
	return EBTNodeResult::Succeeded;
}
