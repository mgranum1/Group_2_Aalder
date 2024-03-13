// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindRandomLocation.h"
#include "NavigationSystem.h"
#include "NPC_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find a Random location in NavMesh";
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Return AI controls and its NPC

	if(auto* const Cont = Cast<ANPC_AIController>(OwnerComp.GetAIOwner()))
	{
		if(auto* const npc = Cont->GetPawn())
		{
			//Obtain NPC location to use as an original location
			auto const Origin = npc->GetActorLocation();

			//Gives us a random location and nav system
			if(auto* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				FNavLocation Loc;
				if(NavSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, Loc))
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Loc.Location);

				}

				//Finish
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	//return EBTNodeResult::Failed;

	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
