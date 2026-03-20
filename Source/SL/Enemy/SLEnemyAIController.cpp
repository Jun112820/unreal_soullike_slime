// Fill out your copyright notice in the Description page of Project Settings.


#include "SL/Enemy/SLEnemyAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "SL/Player/SLHeroController.h"

ASLEnemyAIController::ASLEnemyAIController()
{
	
}

void ASLEnemyAIController::OnUnderAttack(AActor* Attacker)
{
	if (!Attacker || bTargetFound) return;

	UBlackboardComponent* BB = GetBlackboardComponent();
	if (BB)
	{
		BB->SetValueAsObject(TEXT("TargetActor"), Attacker);
	}

	auto AttackerPawn = Cast<APawn>(Attacker);
	if (!AttackerPawn) return;
	
	if (ASLHeroController* PC = Cast<ASLHeroController>(AttackerPawn->GetController()))
	{
		PC->ShowBossHPBar(GetPawn());
	}

	bTargetFound = true;
}

void ASLEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
	}
}