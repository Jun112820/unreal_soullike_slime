// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SL/Abilities/SLGameplayAbility.h"
#include "GA_Pickup.generated.h"

/**
 * 
 */
UCLASS()
class SL_API UGA_Pickup : public USLGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Pickup();
	
protected:
	virtual void ActivateAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
					FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION()
	void OnMontageCompleted();
	
	UFUNCTION()
	void OnMontageInterrupted();
	
protected:
	UPROPERTY(EditAnywhere, Category="GAS")
	TObjectPtr<class UAnimMontage> PickupMontage;
};