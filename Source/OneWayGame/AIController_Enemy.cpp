// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController_Enemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

AAIController_Enemy::AAIController_Enemy()
{
	// Inicializa el componente Blackboard
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	bWantsPlayerState = true;

	
}

void AAIController_Enemy::BeginPlay()
{
	Super::BeginPlay();

	if (BehaviorTree)
	{
		if (BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		}
		RunBehaviorTree(BehaviorTree);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AIController_Enemy: No se ha asignado Behavior Tree"));
	}
}
