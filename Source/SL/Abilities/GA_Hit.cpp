// Fill out your copyright notice in the Description page of Project Settings.


#include "SL/Abilities/GA_Hit.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

void UGA_Hit::ActivateAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                              const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (HitMontage)
	{
		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		  this, NAME_None, HitMontage, 1.0f, NAME_None, false, 1.0f
		);

		MontageTask->OnCompleted.AddDynamic(this, &UGA_Hit::OnMontageCompleted);
		MontageTask->OnBlendOut.AddDynamic(this, &UGA_Hit::OnMontageCompleted);
		MontageTask->OnInterrupted.AddDynamic(this, &UGA_Hit::OnMontageInterrupted);
		MontageTask->OnCancelled.AddDynamic(this, &UGA_Hit::OnMontageInterrupted);

		MontageTask->ReadyForActivation();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}

void UGA_Hit::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Hit::OnMontageCompleted()
{
	K2_EndAbility();
}

void UGA_Hit::OnMontageInterrupted()
{
	K2_EndAbility();
}
