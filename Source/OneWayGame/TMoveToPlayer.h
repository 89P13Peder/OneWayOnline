// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TMoveToPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ONEWAYGAME_API UTMoveToPlayer : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UTMoveToPlayer();

protected:
	virtual EBTNodeResult::Type ExecuteTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetLocationKey;
	
};
