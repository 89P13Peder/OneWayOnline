#include "BTTaskNode_RoamAround.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"


UBTTaskNode_RoamAround::UBTTaskNode_RoamAround()
{
	NodeName = "Patrullaje";
}

EBTNodeResult::Type UBTTaskNode_RoamAround::ExecuteTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	APawn* AIPawn = AIController->GetPawn();
	if (!AIController) return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(AIController->GetWorld());
	if (!NavSys) return EBTNodeResult::Failed;

	FVector Origin = AIPawn->GetActorLocation();
	FNavLocation RandomPoint;

	bool bFound = NavSys->GetRandomReachablePointInRadius(Origin, RoamRadius, RandomPoint);

	if (bFound)
	{
		
		EPathFollowingRequestResult::Type MoveResult = AIController->MoveToLocation(RandomPoint.Location);

		if (MoveResult == EPathFollowingRequestResult::RequestSuccessful)
		{
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}