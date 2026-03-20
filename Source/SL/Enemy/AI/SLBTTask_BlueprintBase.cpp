// Fill out your copyright notice in the Description page of Project Settings.


#include "SL/Enemy/AI/SLBTTask_BlueprintBase.h"
#include "BehaviorTree/BlackboardComponent.h"


AActor* USLBTTask_BlueprintBase::GetSelfActor(UBehaviorTreeComponent* OwnerComp)
{
	if (auto MyBlackboard = OwnerComp->GetBlackboardComponent())
	{
		UObject* BlackBoardObject = MyBlackboard->GetValueAsObject(FName("SelfActor"));
		return Cast<AActor>(BlackBoardObject);
	}

	return nullptr;
}