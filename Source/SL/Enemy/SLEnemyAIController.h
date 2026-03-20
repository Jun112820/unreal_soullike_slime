// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SLEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class SL_API ASLEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	ASLEnemyAIController();
	void OnUnderAttack(AActor* Attacker);
	
protected:
	virtual void OnPossess(APawn* InPawn) override;

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	bool bTargetFound = false;
};