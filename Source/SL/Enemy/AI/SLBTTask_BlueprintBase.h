// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "SLBTTask_BlueprintBase.generated.h"

/**
 * 
 */
UCLASS()
class SL_API USLBTTask_BlueprintBase : public UBTTask_BlueprintBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	AActor* GetSelfActor(UBehaviorTreeComponent* OwnerComp);
	
};
