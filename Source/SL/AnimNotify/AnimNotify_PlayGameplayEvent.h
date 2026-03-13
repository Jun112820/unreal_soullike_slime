// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_PlayGameplayEvent.generated.h"

/**
 * 
 */
UCLASS()
class SL_API UAnimNotify_PlayGameplayEvent : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_PlayGameplayEvent();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, Category = "GAS")
	FGameplayTag EventTag;
};