// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_RoamAround.generated.h"

/**
 * 
 */
UCLASS()
class ONEWAYGAME_API UBTTaskNode_RoamAround : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_RoamAround();

protected:
	virtual EBTNodeResult::Type ExecuteTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	//radio dentro del cual tomar el punto aleatorio
	UPROPERTY(EditAnywhere, Category = "AI")
	float RoamRadius = 1000.0f;
	
};
