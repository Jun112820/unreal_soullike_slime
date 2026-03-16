// Fill out your copyright notice in the Description page of Project Settings.


#include "SL/Abilities/GA_Pickup.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "SL/Character/QuickSlotComponent.h"
#include "SL/Item/SLItem.h"
#include "SL/Util/SLLogChannels.h"

UGA_Pickup::UGA_Pickup()
{
	
}

void UGA_Pickup::ActivateAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (auto* QuickSlotComp = GetAvatarActorFromActorInfo()->FindComponentByClass<UQuickSlotComponent>())
	{
		auto TargetItemActor = Cast<ASLItem>(QuickSlotComp->GetCurrentOverlapItem());
		if (!TargetItemActor) { EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

		QuickSlotComp->AddItem(TargetItemActor);
	}

	if (PickupMontage)
	{
		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			NAME_None,
			PickupMontage,
			1.0f,
			NAME_None,
			false,
			1.0f
		);

		MontageTask->OnCompleted.AddDynamic(this, &UGA_Pickup::OnMontageCompleted);
		MontageTask->OnBlendOut.AddDynamic(this, &UGA_Pickup::OnMontageCompleted);
		MontageTask->OnInterrupted.AddDynamic(this, &UGA_Pickup::OnMontageInterrupted);
		MontageTask->OnCancelled.AddDynamic(this, &UGA_Pickup::OnMontageInterrupted);

		MontageTask->ReadyForActivation();
	}
	else
	{
		UE_LOG(LogSL, Error, TEXT("PickupMontage 가 설정되지 않았습니다!"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}

void UGA_Pickup::EndAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Pickup::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_Pickup::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
