// Fill out your copyright notice in the Description page of Project Settings.


#include "SL/Abilities/GA_MeleeAttack.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/Character.h"
#include "SL/Interface/IDamageable.h"
#include "SL/Util/SLLogChannels.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "SLAbilitySystemComponent.h"

UGA_MeleeAttack::UGA_MeleeAttack(const FObjectInitializer& ObjectInitializer)
{
	ActivationPolicy = ESLAbilityActivationPolicy::InputTriggeredOnce;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_MeleeAttack::ActivateAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (AttackMontage)
	{
		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		   this, NAME_None, AttackMontage, 1.0f, NAME_None, false, 1.0f
		);
		MontageTask->OnCompleted.AddDynamic(this, &UGA_MeleeAttack::OnMontageCompleted);
		MontageTask->OnBlendOut.AddDynamic(this, &UGA_MeleeAttack::OnMontageCompleted);
		MontageTask->OnInterrupted.AddDynamic(this, &UGA_MeleeAttack::OnMontageInterrupted);
		MontageTask->OnCancelled.AddDynamic(this, &UGA_MeleeAttack::OnMontageInterrupted);
		MontageTask->ReadyForActivation();

		USLAbilitySystemComponent* MyASC = Cast<USLAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
		MyASC->ApplyItemEffect(BlockRegenStaminaEffectClass);

		UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this, 
			FGameplayTag::RequestGameplayTag(TEXT("Event.Montage.MeleeAttack")),
			nullptr, 
			false, 
			false
		);

		WaitEventTask->EventReceived.AddDynamic(this, &UGA_MeleeAttack::OnHitEventReceived);
		WaitEventTask->ReadyForActivation();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}

void UGA_MeleeAttack::OnHitEventReceived(FGameplayEventData Payload)
{
	PerformMeleeTrace();
}

bool UGA_MeleeAttack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGA_MeleeAttack::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_MeleeAttack::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_MeleeAttack::PerformMeleeTrace()
{
    AActor* Avatar = GetAvatarActorFromActorInfo();
    UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
    
    if (!Avatar || !SourceASC || !DamageEffectClass) return;
    
    FVector Start = Avatar->GetActorLocation() + Avatar->GetActorForwardVector() * 50.f;
    FVector End = Start + Avatar->GetActorForwardVector() * AttackRange;
    TArray<FHitResult> HitResults;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Avatar);
    
    bool bHit = GetWorld()->SweepMultiByChannel(
        HitResults, Start, End, FQuat::Identity, 
        ECC_Pawn, FCollisionShape::MakeSphere(AttackRadius), Params
    );

	DrawDebugSphere(GetWorld(), Start, AttackRadius, 24, FColor::Red, false, 1.0f);

	if (bHit)
	{
		FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
		ContextHandle.AddHitResult(HitResults[0]); 
    
		FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), ContextHandle);
    
		if (SpecHandle.IsValid())
		{
			TArray<AActor*> HitActors;

			for (const FHitResult& Hit : HitResults)
			{
				AActor* TargetActor = Hit.GetActor();

				if (!TargetActor || HitActors.Contains(TargetActor))
				{
					continue;
				}

				if (IDamageable* Damagable = Cast<IDamageable>(TargetActor))
				{
					UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
                
					if (TargetASC && TargetASC != SourceASC)
					{
						SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
						HitActors.Add(TargetActor);
						UE_LOG(LogSL, Warning, TEXT("Hit Actor (Applied GE): %s"), *TargetActor->GetName());
					}
				}
			}
		}
	}
}