// Fill out your copyright notice in the Description page of Project Settings.


#include "SL/Abilities/GA_ConsumeItem.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/Character.h"
#include "SL/Character/QuickSlotComponent.h"
#include "SL/Util/SLLogChannels.h"

UGA_ConsumeItem::UGA_ConsumeItem()
{
	ActivationPolicy = ESLAbilityActivationPolicy::InputTriggeredOnce;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_ConsumeItem::ActivateAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                      const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!ConsumeItemMontage)
	{
		UE_LOG(LogSL, Error, TEXT("ConsumeItemMontage 가 설정되지 않았습니다!"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ACharacter* Hero = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Hero)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UQuickSlotComponent* QuickSlotComp = Hero->FindComponentByClass<UQuickSlotComponent>();
	if (!QuickSlotComp || !QuickSlotComp->CanUseItem())
	{
		UE_LOG(LogSL, Error, TEXT("현재 아이템이 설정되지 않았습니다!"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	QuickSlotComp->UseCurrentItem();

	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		ConsumeItemMontage,
		1.0f,
		NAME_None,
		false,
		1.0f
	);

	MontageTask->OnCompleted.AddDynamic(this, &UGA_ConsumeItem::OnMontageCompleted);
	MontageTask->OnBlendOut.AddDynamic(this, &UGA_ConsumeItem::OnMontageCompleted);
	MontageTask->OnInterrupted.AddDynamic(this, &UGA_ConsumeItem::OnMontageInterrupted);
	MontageTask->OnCancelled.AddDynamic(this, &UGA_ConsumeItem::OnMontageInterrupted);

	MontageTask->ReadyForActivation();
}

void UGA_ConsumeItem::EndAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_ConsumeItem::OnMontageCompleted()
{
	K2_EndAbility();
}

void UGA_ConsumeItem::OnMontageInterrupted()
{
	K2_EndAbility();
}
