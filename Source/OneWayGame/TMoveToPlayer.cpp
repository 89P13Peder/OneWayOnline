#include "TMoveToPlayer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"

UTMoveToPlayer::UTMoveToPlayer()
{
	NodeName = TEXT("Moverse hacia el jugador");
}

EBTNodeResult::Type UTMoveToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) 
		return EBTNodeResult::Failed;

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) 
		return EBTNodeResult::Failed;

	
	AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject("PlayerActor"));
	if (!Target)
		return EBTNodeResult::Failed;

	
	AIController->MoveToActor(Target, 5.0f);

	return EBTNodeResult::Succeeded;
}
