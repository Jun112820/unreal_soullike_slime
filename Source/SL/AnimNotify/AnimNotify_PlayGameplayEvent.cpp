// Fill out your copyright notice in the Description page of Project Settings.


#include "SL/AnimNotify/AnimNotify_PlayGameplayEvent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

UAnimNotify_PlayGameplayEvent::UAnimNotify_PlayGameplayEvent()
{
}

void UAnimNotify_PlayGameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{
		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MeshComp->GetOwner());
        
		if (ASC && EventTag.IsValid())
		{
			FGameplayEventData Payload;
			Payload.EventTag = EventTag;
			Payload.Instigator = MeshComp->GetOwner();
			Payload.Target = MeshComp->GetOwner();

			ASC->HandleGameplayEvent(EventTag, &Payload);
		}
	}
}
