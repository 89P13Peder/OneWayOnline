// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIController_Enemy.generated.h"

/**
 * 
 */
UCLASS()
class ONEWAYGAME_API AAIController_Enemy : public AAIController
{
	GENERATED_BODY()

public:
	AAIController_Enemy();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="AI")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	class UBlackboardComponent* BlackboardComp;
	
};
